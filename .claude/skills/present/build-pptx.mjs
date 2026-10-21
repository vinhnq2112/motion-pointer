// Build an editable PowerPoint file from a simple Markdown slide source.
// Usage: node build-pptx.mjs <slides.md> <out.pptx>
// Needs the npm package "pptxgenjs" installed in the folder of <slides.md> (npm install pptxgenjs).
//
// Source format (see SKILL.md):
//   --- front matter (title, subtitle, author, date) ---
//   # Slide title          starts a new slide
//   - bullet / "  - sub"   bullet points (two spaces per level, max 2 levels)
//   plain line             paragraph; a slide with only one paragraph becomes a big statement slide
//   ![caption](path)       image (path relative to slides.md); text goes left, image right
//   ```...```              code block (monospace)
//   Note: text             speaker notes (may repeat)
//   **bold**               bold text inside a line
import { readFileSync } from 'node:fs';
import { dirname, resolve, join } from 'node:path';
import { createRequire } from 'node:module';

const [src, out] = process.argv.slice(2);
if (!src || !out) { console.error('Usage: node build-pptx.mjs <slides.md> <out.pptx>'); process.exit(2); }
const baseDir = dirname(resolve(src));
let PptxGenJS;
try { PptxGenJS = createRequire(join(baseDir, 'package.json'))('pptxgenjs'); }
catch { console.error(`pptxgenjs not found. Run: npm install pptxgenjs  (in ${baseDir})`); process.exit(3); }

// ---- parse ----
const text = readFileSync(src, 'utf8').replace(/^﻿/, '').replace(/\r\n/g, '\n');
const meta = {};
let body = text;
const fm = text.match(/^---\n([\s\S]*?)\n---\n/);
if (fm) {
  for (const l of fm[1].split('\n')) { const m = l.match(/^(\w+):\s*(.*)$/); if (m) meta[m[1]] = m[2].trim(); }
  body = text.slice(fm[0].length);
}
const slides = [];
let cur = null, inCode = false;
for (const line of body.split('\n')) {
  if (inCode) { if (line.startsWith('```')) inCode = false; else cur.code.push(line); continue; }
  const h = line.match(/^#\s+(.*)$/);
  if (h) { cur = { title: h[1].trim(), items: [], images: [], code: [], notes: [] }; slides.push(cur); continue; }
  if (!cur) continue;
  if (line.startsWith('```')) { inCode = true; continue; }
  const img = line.match(/^!\[(.*?)\]\((.*?)\)\s*$/);
  if (img) { cur.images.push({ caption: img[1], path: resolve(baseDir, img[2]) }); continue; }
  const note = line.match(/^Note:\s*(.*)$/i);
  if (note) { cur.notes.push(note[1]); continue; }
  const b = line.match(/^(\s*)[-*]\s+(.*)$/);
  if (b) { cur.items.push({ text: b[2], level: Math.min(1, Math.floor(b[1].length / 2)), bullet: true }); continue; }
  if (line.trim()) cur.items.push({ text: line.trim(), level: 0, bullet: false });
}

// ---- style ----
const C = { ink: '1F2933', muted: '52606D', accent: '2563EB', bg: 'FFFFFF', codeBg: 'F1F5F9' };
const FONT = meta.font || 'Calibri';
const runs = (s, opts = {}) => s.split(/(\*\*[^*]+\*\*)/).filter(Boolean).map(p =>
  p.startsWith('**') ? { text: p.slice(2, -2), options: { ...opts, bold: true } } : { text: p, options: { ...opts } });

// ---- build ----
const pres = new PptxGenJS();
pres.layout = 'LAYOUT_WIDE'; // 13.33 x 7.5 in
pres.title = meta.title || 'Presentation';
if (meta.author) pres.author = meta.author;

if (meta.title) {
  const s = pres.addSlide(); s.background = { color: C.bg };
  s.addShape(pres.ShapeType.rect, { x: 0, y: 0, w: 0.25, h: 7.5, fill: { color: C.accent }, line: { color: C.accent } });
  s.addText(meta.title, { x: 0.9, y: 2.3, w: 11.5, h: 1.4, fontFace: FONT, fontSize: 44, bold: true, color: C.ink });
  if (meta.subtitle) s.addText(meta.subtitle, { x: 0.9, y: 3.7, w: 11.5, h: 0.8, fontFace: FONT, fontSize: 24, color: C.muted });
  const by = [meta.author, meta.date].filter(Boolean).join(' · ');
  if (by) s.addText(by, { x: 0.9, y: 5.6, w: 11.5, h: 0.5, fontFace: FONT, fontSize: 16, color: C.muted });
}

for (const d of slides) {
  const s = pres.addSlide(); s.background = { color: C.bg };
  s.addText(d.title, { x: 0.6, y: 0.35, w: 12.1, h: 0.9, fontFace: FONT, fontSize: 32, bold: true, color: C.ink });
  s.addShape(pres.ShapeType.line, { x: 0.6, y: 1.25, w: 1.2, h: 0, line: { color: C.accent, width: 3 } });
  const hasImg = d.images.length > 0, hasCode = d.code.length > 0;
  const textW = hasImg ? 5.9 : 12.1;
  let y = 1.6;
  const statement = !hasImg && !hasCode && d.items.length === 1 && !d.items[0].bullet;
  if (statement) {
    s.addText(runs(d.items[0].text), { x: 1.2, y: 2.2, w: 10.9, h: 3.2, fontFace: FONT, fontSize: 36, color: C.ink, align: 'center', valign: 'middle' });
  } else if (d.items.length) {
    const h = hasCode ? Math.min(2.6, 0.3 + d.items.length * 0.5) : 5.4;
    const paras = d.items.flatMap(it => {
      const r = runs(it.text, { fontSize: it.level ? 18 : 22, color: it.level ? C.muted : C.ink });
      r[0].options.bullet = it.bullet ? { indent: 20 } : false;
      r[0].options.indentLevel = it.level;
      r[r.length - 1].options.breakLine = true;
      r[0].options.paraSpaceAfter = 8;
      return r;
    });
    s.addText(paras, { x: 0.6, y, w: textW, h, fontFace: FONT, valign: 'top' });
    y += h + 0.2;
  }
  if (hasCode) {
    const codeH = Math.min(7.0 - y, 0.4 + d.code.length * 0.3);
    s.addText(d.code.join('\n'), { x: 0.6, y, w: textW, h: codeH, fontFace: 'Consolas', fontSize: 16, color: C.ink,
      fill: { color: C.codeBg }, valign: 'top', margin: 10 });
  }
  if (hasImg) {
    const n = d.images.length, slotH = 5.4 / n;
    d.images.forEach((im, i) => {
      const top = 1.6 + i * slotH, capH = im.caption ? 0.4 : 0;
      s.addImage({ path: im.path, x: 6.8, y: top, w: 5.9, h: slotH - capH - 0.1, sizing: { type: 'contain', w: 5.9, h: slotH - capH - 0.1 } });
      if (im.caption) s.addText(im.caption, { x: 6.8, y: top + slotH - capH - 0.1, w: 5.9, h: capH, fontFace: FONT, fontSize: 12, color: C.muted, align: 'center' });
    });
  }
  if (d.notes.length) s.addNotes(d.notes.join('\n'));
}

await pres.writeFile({ fileName: resolve(out) });
console.log(`Wrote ${resolve(out)} — ${slides.length + (meta.title ? 1 : 0)} slides`);
