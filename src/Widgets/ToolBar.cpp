#include "..\..\includes\Widgets\ToolBar.hpp"

ToolBar::ToolBar(QWidget * parent) : 
	QToolBar(parent)
{
	addAction(m_paramAction);
	addAction(m_removeAction);
	addSeparator();
	addSeparator();
}

ToolBar::~ToolBar()
{

}

void ToolBar::reset()
{

}
