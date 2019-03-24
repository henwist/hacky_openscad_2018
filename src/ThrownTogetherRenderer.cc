/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ThrownTogetherRenderer.h"
#include "polyset.h"
#include "printutils.h"
#include <QtGui/QOpenGLFunctions>

#include "system-gl.h"

ThrownTogetherRenderer::ThrownTogetherRenderer(shared_ptr<CSGProducts> root_products,
																							 shared_ptr<CSGProducts> highlight_products,
																							 shared_ptr<CSGProducts> background_products)
	: root_products(root_products), highlight_products(highlight_products), background_products(background_products)
{
  //QOpenGLFunctions::initializeOpenGLFunctions();
}

void ThrownTogetherRenderer::draw(bool /*showfaces*/, bool showedges) const
{
  QOpenGLFunctions *func = new QOpenGLFunctions();
  const_cast<QOpenGLFunctions*>(func)->initializeOpenGLFunctions();

	PRINTD("Thrown draw");
 	if (this->root_products) {
		func->glEnable(GL_CULL_FACE);
		func->glCullFace(GL_BACK);
		renderCSGProducts(*this->root_products, false, false, showedges, false);
		func->glCullFace(GL_FRONT);
		//glColor3ub(255, 0, 255);
		renderCSGProducts(*this->root_products, false, false, showedges, true);
		func->glDisable(GL_CULL_FACE);
	}
	if (this->background_products)
	 	renderCSGProducts(*this->background_products, false, true, showedges, false);
	if (this->highlight_products)
	 	renderCSGProducts(*this->highlight_products, true, false, showedges, false);
}

void ThrownTogetherRenderer::renderChainObject(const CSGChainObject &csgobj, bool highlight_mode,
																							 bool background_mode, bool showedges, bool fberror, OpenSCADOperator type) const
{
  QOpenGLFunctions *func = new QOpenGLFunctions();
  const_cast<QOpenGLFunctions*>(func)->initializeOpenGLFunctions();

	if (this->geomVisitMark[std::make_pair(csgobj.leaf->geom.get(), &csgobj.leaf->matrix)]++ > 0) return;
	const Color4f &c = csgobj.leaf->color;
	csgmode_e csgmode = get_csgmode(highlight_mode, background_mode, type);
	ColorMode colormode = ColorMode::NONE;
	ColorMode edge_colormode = ColorMode::NONE;
	
	if (highlight_mode) {
		colormode = ColorMode::HIGHLIGHT;
		edge_colormode = ColorMode::HIGHLIGHT_EDGES;
	} else if (background_mode) {
		if (csgobj.flags & CSGNode::FLAG_HIGHLIGHT) {
			colormode = ColorMode::HIGHLIGHT;
		}
		else {
			colormode = ColorMode::BACKGROUND;
		}
		edge_colormode = ColorMode::BACKGROUND_EDGES;
	} else if (fberror) {
	} else if (type == OpenSCADOperator::DIFFERENCE) {
		if (csgobj.flags & CSGNode::FLAG_HIGHLIGHT) {
			colormode = ColorMode::HIGHLIGHT;
		}
		else {
			colormode = ColorMode::CUTOUT;
		}
		edge_colormode = ColorMode::CUTOUT_EDGES;
	} else {
		if (csgobj.flags & CSGNode::FLAG_HIGHLIGHT) {
			colormode = ColorMode::HIGHLIGHT;
		}
		else {
			colormode = ColorMode::MATERIAL;
		}
		edge_colormode = ColorMode::MATERIAL_EDGES;
	}
	
	const Transform3d &m = csgobj.leaf->matrix;
	setColor(colormode, c.data());
	//func->glPushMatrix();
	//func->glMultMatrixd(m.data());
	render_surface(csgobj.leaf->geom, csgmode, m);
	if (showedges) {
		// FIXME? glColor4f((c[0]+1)/2, (c[1]+1)/2, (c[2]+1)/2, 1.0);
		setColor(edge_colormode);
		render_edges(csgobj.leaf->geom, csgmode);
	}
	//func->glPopMatrix();
	
}

void ThrownTogetherRenderer::renderCSGProducts(const CSGProducts &products, bool highlight_mode,
																							 bool background_mode, bool showedges, 
																							 bool fberror) const
{
    QOpenGLFunctions *func = new QOpenGLFunctions();
    const_cast<QOpenGLFunctions*>(func)->initializeOpenGLFunctions();

	PRINTD("Thrown renderCSGProducts");
	func->glDepthFunc(GL_LEQUAL);
	this->geomVisitMark.clear();

	for(const auto &product : products.products) {
		for(const auto &csgobj : product.intersections) {
			renderChainObject(csgobj, highlight_mode, background_mode, showedges, fberror, OpenSCADOperator::INTERSECTION);
		}
		for(const auto &csgobj : product.subtractions) {
			renderChainObject(csgobj, highlight_mode, background_mode, showedges, fberror, OpenSCADOperator::DIFFERENCE);
		}
	}
}

BoundingBox ThrownTogetherRenderer::getBoundingBox() const
{
	BoundingBox bbox;
	if (this->root_products) bbox = this->root_products->getBoundingBox();
	if (this->highlight_products) bbox.extend(this->highlight_products->getBoundingBox());
//	if (this->background_products) bbox.extend(this->background_products->getBoundingBox());
	return bbox;
}
