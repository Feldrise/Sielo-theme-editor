#include "..\..\includes\Widgets\ToolBar.hpp"

ManageToolBar::ManageToolBar(ToolBar * parentToolBar, QWidget * parent) : 
	QDialog(parent),
	m_parentToolBar(parentToolBar)
{
}

ManageToolBar::~ManageToolBar()
{
	// Empty
}


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
	// Empty
}

void ToolBar::reset()
{

}

