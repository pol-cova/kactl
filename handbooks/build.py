#!/usr/bin/env python3
"""Build the two printable companions. Requires reportlab; no LaTeX/network."""
from pathlib import Path
import hashlib
import html
import re
import textwrap
from reportlab.lib import colors
from reportlab.lib.enums import TA_LEFT
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.pagesizes import A4
from reportlab.platypus import (BaseDocTemplate, PageTemplate, Frame, Paragraph,
    Spacer, PageBreak, Preformatted, Table, TableStyle, KeepTogether)
from reportlab.platypus.tableofcontents import TableOfContents

ROOT = Path(__file__).resolve().parent
OUT = ROOT / 'pdf'
OUT.mkdir(exist_ok=True)
INK = colors.HexColor('#142B3D')
ACCENT = colors.HexColor('#15616D')
GRAY = colors.HexColor('#52606B')
styles = getSampleStyleSheet()
styles.add(ParagraphStyle('BodyBook', fontName='Helvetica', fontSize=9.5,
    leading=12.7, spaceAfter=5, textColor=INK))
styles.add(ParagraphStyle('TitleBook', fontName='Helvetica-Bold', fontSize=26,
    leading=30, spaceAfter=14, textColor=INK))
styles.add(ParagraphStyle('H1Book', fontName='Helvetica-Bold', fontSize=17,
    leading=21, spaceBefore=12, spaceAfter=9, textColor=INK, keepWithNext=True))
styles.add(ParagraphStyle('H2Book', fontName='Helvetica-Bold', fontSize=12.5,
    leading=16, spaceBefore=11, spaceAfter=6, textColor=ACCENT, keepWithNext=True))
styles.add(ParagraphStyle('H3Book', fontName='Helvetica-Bold', fontSize=10,
    leading=13, spaceBefore=8, spaceAfter=4, textColor=INK, keepWithNext=True))
styles.add(ParagraphStyle('CodeBook', fontName='Courier', fontSize=7.8,
    leading=9.5, spaceAfter=7, textColor=INK, leftIndent=4,
    borderColor=colors.HexColor('#D6E1E6'), borderWidth=.4, borderPadding=5,
    backColor=colors.HexColor('#F5F8FA')))
styles.add(ParagraphStyle('CellBook', parent=styles['BodyBook'], fontSize=8,
    leading=10.2, spaceAfter=0))
styles.add(ParagraphStyle('SmallBook', parent=styles['BodyBook'], fontSize=8,
    leading=10.5, textColor=GRAY))
styles.add(ParagraphStyle('BulletBook', parent=styles['BodyBook'],
    leftIndent=11, firstLineIndent=-8))

# Standard PDF fonts cover these common punctuation marks poorly in code.
TRANSLATE = str.maketrans({'\u2014':' - ', '\u2013':'-', '\u2011':'-',
    '\u2212':'-', '\u2192':' -> ', '\u2264':'<=', '\u2265':'>=',
    '\u2260':'!=', '\u00d7':'*', '\u00b7':'*', '\u2019':"'", '\u2018':"'",
    '\u201c':'"', '\u201d':'"', '\u03c6':'phi', '\u03bc':'mu', '\u03a3':'sum',
    '\u2211':'sum', '\u221e':'infinity', '\u2308':'ceil(', '\u2309':')',
    '\u230a':'floor(', '\u230b':')', '\u2248':'~', '\u2208':' in '})

def clean(s):
    return s.translate(TRANSLATE)

def inline(s):
    s = clean(s)
    # Protect code before interpreting emphasis or links.
    code = []
    def stash(m):
        code.append('<font name="Courier">'+html.escape(m.group(1))+'</font>')
        return f'ZZCODETOKEN{len(code)-1}ZZ'
    s = re.sub(r'`([^`]+)`', stash, s)
    s = html.escape(s)
    def link(m):
        label, dest = m.groups()
        if dest.startswith(('http://','https://')):
            return f'<link href="{dest}" color="#15616D">{label}</link>'
        # Local paths remain printed text; repo is available offline.
        return label + (f' <font size="7">[{dest}]</font>' if dest != label else '')
    s = re.sub(r'\[([^]]+)\]\(([^)]+)\)', link, s)
    s = re.sub(r'\*\*(.+?)\*\*',r'<b>\1</b>',s)
    for n,v in enumerate(code): s=s.replace(f'ZZCODETOKEN{n}ZZ',v)
    return s

class BookDoc(BaseDocTemplate):
    def __init__(self, filename, title):
        super().__init__(str(filename), pagesize=A4, leftMargin=38,
            rightMargin=38, topMargin=40, bottomMargin=38,
            title=title, author='Mexico finals team / KACTL companions')
        self.book_title = title
        self.addPageTemplates(PageTemplate('body', [Frame(self.leftMargin,
            self.bottomMargin, self.width, self.height, id='normal',
            leftPadding=0,rightPadding=0,topPadding=0,bottomPadding=0)],
            onPage=self.furniture))
    def furniture(self, c, doc):
        c.saveState()
        c.setStrokeColor(colors.HexColor('#BBCDD5'))
        c.line(38,A4[1]-29,A4[0]-38,A4[1]-29)
        c.setFont('Helvetica',7.5);c.setFillColor(GRAY)
        c.drawString(38,A4[1]-23,self.book_title)
        c.drawString(38,23,'Finals companion | C++17 | 18 October 2026')
        c.drawRightString(A4[0]-38,23,str(doc.page))
        c.restoreState()
    def afterFlowable(self, f):
        if hasattr(f,'toc_level'):
            key=f.book_key
            self.canv.bookmarkPage(key)
            self.canv.addOutlineEntry(f.getPlainText(),key,f.toc_level,False)
            if f.toc_level <= 1:
                self.notify('TOCEntry',(f.toc_level,f.getPlainText(),self.page,key))

counter=0

def heading(text, level=1):
    global counter
    counter+=1
    p=Paragraph(inline(text),styles[f'H{min(level,3)}Book'])
    p.toc_level=min(level-1,2);p.book_key=f'section-{counter}'
    return p

def code_flow(text):
    # Wrap long comments and exceptional long expressions at a visible indent.
    # Printed wrapping is a layout aid; the .hpp is the authoritative source.
    lines=[]
    for line in clean(text).expandtabs(2).splitlines():
        if len(line)<=104: lines.append(line)
        else:
            lines.extend(textwrap.wrap(line,width=104,subsequent_indent='    ',
                break_long_words=False,break_on_hyphens=False,
                replace_whitespace=False,drop_whitespace=True) or [''])
    return Preformatted('\n'.join(lines),styles['CodeBook'])

def source_flow(path):
    text=path.read_text()
    if 'content' in path.resolve().parts:
        author=re.search(r'Author: ([^\n]+)',text)
        license_=re.search(r'License: ([^\n]+)',text)
        notice='Source: '+str(path.relative_to(ROOT))
        if author: notice+=' | Author: '+author.group(1).strip()
        if license_: notice+=' | License: '+license_.group(1).strip()
        # The full prose explains the contract. Omit LaTeX-rich source docblocks
        # from print, but preserve attribution and code exactly.
        code=re.sub(r'/\*\*.*?\*/', '', text, flags=re.S).strip()
        block=[heading('KACTL code: '+path.stem,2),
            Paragraph(inline(notice),styles['SmallBook']),code_flow(code)]
        return [KeepTogether(block)] if len(code.splitlines()) <= 65 else block
    chunks=re.split(r'^// BOOK: (.+)$',text,flags=re.M)
    story=[]
    if chunks[0].strip():
        story.append(heading('Code conventions and includes',2))
        story.append(code_flow(chunks[0].strip()))
    for i in range(1,len(chunks),2):
        digest=hashlib.sha256(chunks[i+1].encode()).hexdigest()[:8]
        block=[heading(chunks[i],2),
            Paragraph(inline(f'Source: {path.relative_to(ROOT)} | SHA256 fragment {digest}'),styles['SmallBook']),
            code_flow(chunks[i+1].strip())]
        if len(chunks[i+1].splitlines()) <= 65:
            story.append(KeepTogether(block))
        else:
            story.extend(block)
    return story

def markdown(path):
    lines=path.read_text().splitlines();out=[];i=0;para=[]
    def flush():
        if para:out.append(Paragraph(inline(' '.join(para)),styles['BodyBook']));para.clear()
    while i<len(lines):
        line=lines[i]
        if line.startswith('```'):
            flush();buf=[];i+=1
            while i<len(lines) and not lines[i].startswith('```'):
                buf.append(lines[i]);i+=1
            out.append(code_flow('\n'.join(buf)));i+=1;continue
        m=re.match(r'\{\{code:([^}]+)\}\}',line)
        if m:
            flush();out.extend(source_flow(ROOT/m.group(1)));i+=1;continue
        m=re.match(r'^(#{1,3})\s+(.*)',line)
        if m:
            flush();level=len(m.group(1))
            if level==1 and out:out.append(PageBreak())
            out.append(heading(m.group(2),level));i+=1;continue
        if line.startswith('|'):
            flush();rows=[]
            while i<len(lines) and lines[i].startswith('|'):
                row=[v.strip() for v in lines[i].strip().strip('|').split('|')]
                if not all(re.fullmatch(r'[:\- ]+',v or '-') for v in row):rows.append(row)
                i+=1
            if rows:
                n=max(map(len,rows));data=[]
                for row in rows:
                    row+=['']*(n-len(row))
                    data.append([Paragraph(inline(x),styles['CellBook']) for x in row])
                # Long path columns need space. Equal widths avoid brittle topic-specific rules.
                t=Table(data,colWidths=[(A4[0]-76)/n]*n,repeatRows=1,hAlign='LEFT')
                t.setStyle(TableStyle([('VALIGN',(0,0),(-1,-1),'TOP'),
                    ('BACKGROUND',(0,0),(-1,0),colors.HexColor('#E5EFF2')),
                    ('LINEBELOW',(0,0),(-1,0),.5,ACCENT),
                    ('LINEBELOW',(0,1),(-1,-1),.25,colors.HexColor('#D6E1E6')),
                    ('LEFTPADDING',(0,0),(-1,-1),5),('RIGHTPADDING',(0,0),(-1,-1),5),
                    ('TOPPADDING',(0,0),(-1,-1),5),('BOTTOMPADDING',(0,0),(-1,-1),5)]))
                out.extend([t,Spacer(1,7)])
            continue
        if not line.strip():flush();i+=1;continue
        m=re.match(r'^\s*(?:[-*]|\d+\.)\s+(.*)',line)
        if m:
            flush();out.append(Paragraph('&#8226; '+inline(m.group(1)),styles['BulletBook']));i+=1;continue
        if line.strip()=='---':flush();i+=1;continue
        para.append(line.strip());i+=1
    flush();return out

def build(name,title,files):
    global counter
    counter=0
    toc=TableOfContents()
    toc.levelStyles=[ParagraphStyle('TOC0',fontName='Helvetica-Bold',fontSize=9,
        leading=12,spaceBefore=5,textColor=INK),
        ParagraphStyle('TOC1',fontName='Helvetica',fontSize=8.5,leading=11,
        leftIndent=12,textColor=INK),
        ParagraphStyle('TOC2',fontName='Helvetica',fontSize=8,leading=10,
        leftIndent=24,textColor=GRAY)]
    front=[Spacer(1,40),Paragraph(title,styles['TitleBook']),
        Paragraph('Mexico finals team reference',styles['H2Book']),
        Paragraph('Built from the 2017-2025 regional research and pol-cova/kactl. '
            'A companion to the existing KACTL notebook, with explanations, '
            'contracts, and tested C++17 helpers.',styles['BodyBook']),
        Spacer(1,12),Paragraph('Use the recognition tables first. Check assumptions before typing. '
            'All helper indices are zero-based unless explicitly stated. '
            'Advanced KACTL entries are navigation references; consult their source '
            'and listed dependencies. The team must confirm event material limits '
            'before choosing what to print.',styles['BodyBook']),
        Spacer(1,12),Paragraph('Edition 1 - 18 October 2026<br/>Editable sources and tests: handbooks/ '
            'in the repository.<br/>This document does not replace the original kactl.pdf.',styles['SmallBook']),
        PageBreak(),Paragraph('Contents and code lookup',styles['H1Book']),toc,PageBreak()]
    body=[]
    for f in files:body.extend(markdown(ROOT/f))
    doc=BookDoc(OUT/name,title);doc.multiBuild(front+body)
    print(OUT/name)

if __name__=='__main__':
    build('math-handbook.pdf','Math reasoning and formulas',['math.md'])
    build('implementation-handbook.pdf','Algorithms and implementations',['implementations.md','specialists.md'])
