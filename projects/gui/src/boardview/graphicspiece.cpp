/*
    This file is part of Cute Chess.
    Copyright (C) 2008-2018 Cute Chess authors

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "graphicspiece.h"
#include <QPainter> 
#include <QSvgRenderer>


GraphicsPiece::GraphicsPiece(const Chess::Piece& piece,
			     qreal squareSize,
			     const QString& elementId,
			     bool flippable, 
			     QSvgRenderer* renderer,
			     QGraphicsItem* parent)
	: QGraphicsObject(parent),
	  m_piece(piece),
	  m_rect(-squareSize / 2, -squareSize / 2,
		  squareSize, squareSize),
	  m_elementId(elementId),
	  m_renderer(renderer),
	  m_container(nullptr), 
	  m_flipped(false), 
	  m_flippable(flippable)
{
	setAcceptedMouseButtons(Qt::LeftButton);
	setCacheMode(DeviceCoordinateCache);
}

int GraphicsPiece::type() const
{
	return Type;
}

QRectF GraphicsPiece::boundingRect() const
{
	return m_rect;
}

void GraphicsPiece::paint(QPainter* painter,
			  const QStyleOptionGraphicsItem* option,
			  QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QRectF bounds(m_renderer->boundsOnElement(m_elementId));
	qreal ar = bounds.width() / bounds.height();
	qreal width = m_rect.width() * 0.8;

	if (ar > 1.0)
	{
		bounds.setWidth(width);
		bounds.setHeight(width / ar);
	}
	else
	{
		bounds.setHeight(width);
		bounds.setWidth(width * ar);
	}
	bounds.moveCenter(m_rect.center());

	if (m_flippable && m_flipped) painter->rotate(180); 
	m_renderer->render(painter, m_elementId, bounds);
	if (m_flippable && m_flipped) painter->rotate(-180); 
}

bool GraphicsPiece::isFlipped() const 
{
	return m_flipped; 
}

void GraphicsPiece::setFlipped(bool flipped) 
{
	m_flipped = flipped; 
}

Chess::Piece GraphicsPiece::pieceType() const
{
	return m_piece;
}

QGraphicsItem* GraphicsPiece::container() const
{
	return m_container;
}

void GraphicsPiece::setContainer(QGraphicsItem* item)
{
	m_container = item;
}

void GraphicsPiece::restoreParent()
{
	if (parentItem() == nullptr && m_container != nullptr)
	{
		QPointF point(m_container->mapFromScene(pos()));
		setParentItem(m_container);
		setPos(point);
	}
}
