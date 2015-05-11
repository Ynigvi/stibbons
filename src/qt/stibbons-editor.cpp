#include "stibbons-editor.h"
#include "line-number-area.h"

StibbonsEditor::StibbonsEditor(QWidget *parent) : QPlainTextEdit(parent) {
	setFont(QFont("Monospace",12));
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

	setViewportMargins(areaWidth(), 0, 0, 0);
}

int StibbonsEditor::areaWidth() const {
	return floor(log10(blockCount()) + 1) * fontMetrics().width(QLatin1Char('9'));
}

void StibbonsEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
		setViewportMargins(areaWidth(), 0, 0, 0);
}

void StibbonsEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), areaWidth(), cr.height()));
}

void StibbonsEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);    
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();    
    while (block.isValid() && top <= event->rect().bottom()) {
      if (block.isVisible() && bottom >= event->rect().top()) {
	QString number = QString::number(blockNumber + 1);
	painter.setPen(Qt::black);
	painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
			 Qt::AlignRight, number);
      }

      block = block.next();
      top = bottom;
      bottom = top + (int) blockBoundingRect(block).height();
      ++blockNumber;
    }
}


/*
 * Editor modelines  -  http://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: t
 * truncate-lines: 1
 * End:
 *
 * vim:set ft=cpp ts=4 sw=4 sts=4
 */

