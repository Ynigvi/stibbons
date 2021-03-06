/*
 * This file is part of Stibbons.

 * Stibbons is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Stibbons is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with Stibbons.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file world-view.cpp
 * \brief The Stibbons world view
 * \author Adrien Plazas
 * \version 1.1
 * \date 28/02/2015
 */

#include "world-view.h"

#include <QPainter>
#include <QPaintEvent>

namespace stibbons {

WorldView::WorldView(QWidget *parent) :
	QWidget(parent),
	world(nullptr),
	worldPainter(WorldPainter(nullptr)) {
	connect(this, SIGNAL(changed()), this, SLOT(update()));
}

QSize WorldView::sizeHint() const {
	if (world) {
		auto size = world->getSize();
		return QSize(size.getValue(0), size.getValue(1));
	}

	return QSize();
}

void WorldView::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect boundingRect;
	setMinimumHeight(boundingRect.height());

	worldPainter.paint(painter);
}

void WorldView::setWorld(WorldPtr world) {
	this->world = world;

	resize(sizeHint());
	updateGeometry();

	worldPainter = WorldPainter(world);

	world->onChanged([this]() {
		emit changed();
	});
}

WorldPtr WorldView::getWorld() {
	return world;
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
