/**
 * \file window.cpp
 * \brief The Stibbons main window
 * \author Adrien Plazas
 * \version 0.1
 * \date 26/02/2015
 */

#include "window.h"

#include <QtWidgets>

namespace Stibbons {

Window::Window() {
	createActions();
	createToolBars();

	readSettings();

	setUnifiedTitleAndToolBarOnMac(true);
}

void Window::createActions() {
	QIcon icon;

	icon = QApplication::style()->standardIcon (QStyle::SP_DialogOpenButton);
	openAct = new QAction(icon, tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	icon = QApplication::style()->standardIcon (QStyle::SP_MediaPlay);
	runAct = new QAction(icon, tr("&Run..."), this);
	runAct->setShortcuts(QKeySequence::Open);
	runAct->setStatusTip(tr("Run the program"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(run()));

	icon = QApplication::style()->standardIcon (QStyle::SP_MediaPause);
	haltAct = new QAction(icon, tr("&Halt..."), this);
	haltAct->setShortcuts(QKeySequence::Open);
	haltAct->setStatusTip(tr("Halt the execution of the program"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(halt()));

	quitAct = new QAction(tr("&Quit"), this);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Quit the application"));
	connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void Window::createToolBars() {
	toolbar = addToolBar(tr("Toolbar"));
	toolbar->setFloatable(false);
	toolbar->setMovable(false);

	// Boutons
	toolbar->addAction(openAct);
	toolbar->addAction(runAct);
	toolbar->addAction(haltAct);

	// Extenseur
	QWidget* empty = new QWidget();
	empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	toolbar->addWidget(empty);

	// Bouton menu
	QMenu *menu = new QMenu(tr("Menu"));
	menu->addAction(aboutAct);
	menu->addAction(quitAct);
	toolbar->addAction(menu->menuAction());
	menu->menuAction()->setIcon(QIcon(":/images/open-menu.svg"));
//	menu->menuAction()->setIcon(QApplication::style()->standardIcon (QStyle::SP_ArrowDown));
}

void Window::closeEvent(QCloseEvent *event) {
	writeSettings();
}

void Window::open() {
	QString fileName = QFileDialog::getOpenFileName(this);
	// TODO charger le fichier
}

void Window::run() {
	// TODO démarrer le programme
}

void Window::halt() {
	// TODO suspend l'exécution du programme
}

void Window::about() {
	QMessageBox::about(this, tr("About Stibbons"),
		tr(
			"<p><b>Stibbons</b> multi agent-system</p>"
			"<p><a href=\"https://gitlab.info-ufr.univ-montp2.fr/florian.galinier/stibbons\">Site Web</a></p>"
			"<p><small>"
				"Copyright © 2015 <a href=\"mailto://juliasupernana@neuf.fr\">Julia Bassoumi</a><br/>"
				"Copyright © 2015 <a href=\"mailto://galinierflo@gmail.com\">Florian Galinier</a><br/>"
				"Copyright © 2015 <a href=\"mailto://kekun.plazas@laposte.net\">Adrien Plazas</a><br/>"
				"Copyright © 2015 <a href=\"mailto://clem.simon.sc@gmail.com\">Clement Simon</a>"
			"</small></p>"
		));
}

void Window::readSettings() {
	QSettings settings;
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void Window::writeSettings() {
	QSettings settings;
	settings.setValue("pos", pos());
	settings.setValue("size", size());
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
