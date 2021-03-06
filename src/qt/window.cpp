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
 * \file window.cpp
 * \brief The Stibbons main window
 * \author Adrien Plazas
 * \version 1.1
 * \date 26/02/2015
 */

#include "window.h"

#include <iostream>

#include <cstdio>
#include <unistd.h>

namespace stibbons {

Window::Window() : runner(nullptr) {
	tab=nullptr;
	createActions();
	createToolBars();

	auto scrollArea = new QScrollArea();
	scrollArea->setAlignment(Qt::AlignCenter);
	scrollArea->show();

	worldView = new WorldView(nullptr);
	worldView->show();

	setCentralWidget(scrollArea);
	scrollArea->setWidget(worldView);

	readSettings();

	setUnifiedTitleAndToolBarOnMac(true);

	updateToolbar();

	createTab(scrollArea);

}

Window::~Window() {
	if (runner != nullptr) {
		delete runner;
		runner = nullptr;
	}
}

void Window::createActions() {
	QIcon icon;

	icon = QApplication::style()->standardIcon (QStyle::SP_DialogOpenButton);
	openAct = new QAction(icon, tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	icon = QApplication::style()->standardIcon (QStyle::SP_BrowserReload);
	resetAct = new QAction(icon, tr("&Reset"), this);
	resetAct->setShortcut(QKeySequence(tr("F5")));
	resetAct->setStatusTip(tr("Reset the world..."));
	connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

	icon = QApplication::style()->standardIcon (QStyle::SP_MediaPlay);
	runAct = new QAction(icon, tr("&Run"), this);
	runAct->setShortcut(QKeySequence(tr("Ctrl+Space")));
	runAct->setStatusTip(tr("Run the program"));
	connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

	icon = QApplication::style()->standardIcon (QStyle::SP_MediaPlay);
	rerunAct = new QAction(icon, tr("&Reset and run"), this);
	rerunAct->setShortcut(QKeySequence(tr("Ctrl+R")));
	rerunAct->setStatusTip(tr("Reset and run the program"));
	connect(rerunAct, SIGNAL(triggered()), this, SLOT(rerun()));

	icon = QApplication::style()->standardIcon (QStyle::SP_DialogSaveButton);
	saveAct = new QAction(icon, tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the program"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	icon = QApplication::style()->standardIcon (QStyle::SP_MediaPause);
	haltAct = new QAction(icon, tr("&Halt"), this);
	haltAct->setShortcut(QKeySequence(tr("Ctrl+Space")));
	haltAct->setStatusTip(tr("Halt the execution of the program"));
	connect(haltAct, SIGNAL(triggered()), this, SLOT(halt()));

	quitAct = new QAction(tr("&Quit"), this);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Quit the application"));
	connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	exportAct = new QAction(tr("&Export..."), this);
	exportAct->setShortcut(QKeySequence(tr("Ctrl+E")));
	exportAct->setStatusTip(tr("Export the model to a file"));
	connect(exportAct, SIGNAL(triggered()), this, SLOT(exportModel()));

	saveUnderAct = new QAction(tr("&Save as..."), this);
	saveUnderAct->setShortcuts(QKeySequence::SaveAs);
	saveUnderAct->setStatusTip(tr("Save the program in a file"));
	connect(saveUnderAct, SIGNAL(triggered()), this, SLOT(saveUnder()));

	new QShortcut(QKeySequence::NextChild, this, SLOT(nextTab()));
	new QShortcut(QKeySequence::PreviousChild, this, SLOT(previousTab()));
}

void Window::createToolBars() {
	toolbar = addToolBar(tr("Toolbar"));
	toolbar->setFloatable(false);
	toolbar->setMovable(false);

	// Boutons
	toolbar->addAction(openAct);
	toolbar->addAction(resetAct);
	toolbar->addAction(runAct);
	toolbar->addAction(haltAct);
	toolbar->addAction(saveAct);

	// Extenseur
	QWidget* empty = new QWidget();
	empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	toolbar->addWidget(empty);

	// Slider
	slider = new QSlider(Qt::Horizontal);
	slider->setTickInterval(100);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setMaximum(400);
	slider->setInvertedAppearance(true);
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateInterpreterWaitTime(int)));
	slider->setValue(200);
	toolbar->addWidget(slider);

	// Bouton menu
	QMenu *menu = new QMenu(tr("Menu"));
	menu->addAction(rerunAct);
	menu->addSeparator();
	menu->addAction(saveUnderAct);
	menu->addAction(exportAct);
	menu->addSeparator();
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
	bool restart = runner ? runner->isRunning() : false;

	if (restart)
		runner->halt();

	QString fileName = QFileDialog::getOpenFileName(this);

	if (restart)
		runner->start();

	loadFile(fileName);

	updateToolbar();
}

void Window::loadFile(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		//QMessageBox::warning(this, tr("Application"),
		//                     tr("Cannot read file %1:\n%2.")
		//                     .arg(fileName)
		//                     .arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
	program = in.readAll().toStdString();

	loadProgram();

	loadText(fileName);
	tab->setCurrentWidget(code);
}

void Window::loadText(QString fileName){

	textArea->setGeometry(100,100,400,200);
	QFile fichier(fileName);

	if(fichier.open(QIODevice::ReadWrite))
	{
		textArea->setPlainText(fichier.readAll());
		fichier.close();
		openFileName=fileName;
	}
	else cout<<"Impossible d'ouvrir le fichier !"<<endl;
	//TODO Lever une exception

	textArea->show();
	textArea->setFocus();

}

void Window::loadProgram() {
	if (runner != nullptr) {
		delete runner;
		runner = nullptr;
	}

	auto scrollArea = new QScrollArea();
	try {
		runner = new Runner(program);

		connect(runner, SIGNAL(error(QString,QString)),
			    this, SLOT(error(QString,QString)));

		scrollArea->setAlignment(Qt::AlignCenter);
		scrollArea->show();

		worldView = new WorldView(nullptr);
		worldView->setWorld(runner->getWorld());
		worldView->show();
		setCentralWidget(scrollArea);
		scrollArea->setWidget(worldView);
	}
	catch (SemanticException e) {
		error("Semantic error", QString(e.what()));
	}
	catch (SyntaxException e) {
		error("Syntax error", QString(e.what()));
	}
	catch (exception e) {
		error("Unknown error", QString(e.what()));
	}

	createTab(scrollArea);

	if (slider)
		updateInterpreterWaitTime(slider->value());
	updateToolbar();
}

void Window::reset() {
	program=textArea->document()->toPlainText().toStdString(); 
	loadProgram();
}

void Window::run() {
	if (!runner || runner->isRunning())
		return;

	//save();
	tab->setCurrentWidget(print);

	if (runner)
		runner->start();

	updateToolbar();
}

void Window::rerun() {
	reset();
	run();
}

void Window::halt() {
	if (!runner || !runner->isRunning())
		return;

	if (runner)
		runner->halt();

	updateToolbar();
}

void Window::save(){
if (openFileName != "")
{
	QFile code(openFileName);
	code.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream flux(&code);
	flux<< textArea->toPlainText();
	code.close();

	updateToolbar();
} else { saveUnder();}
}

void Window::saveUnder(){
	openFileName = QFileDialog::getSaveFileName(this, tr("Save File"));
	QFile code(openFileName);
	code.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream flux(&code);
	flux<< textArea->toPlainText();
	code.close();

	updateToolbar();
}

void Window::exportModel() {
	if (runner) {
		bool restart = runner->isRunning();

		if (restart)
			runner->halt();

		QString fileName = QFileDialog::getSaveFileName(this);

		if (restart)
			runner->start();

		exportModel(fileName);
	}
}

void Window::exportModel(QString fileName) {
	if (runner) {
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("Application"),
				                 tr("Cannot open file %1:\n%2.")
				                 .arg(fileName)
				                 .arg(file.errorString()));
			return;
		}

		auto exported = runner->exportModel();

		file.write(exported.c_str());
	}
}

void Window::about() {
	QMessageBox::about(this, tr("About Stibbons"),
		tr(
			"<p><b>Stibbons</b> multi agent-system</p>"
			"<p><a href=\"https://gitlab.info-ufr.univ-montp2.fr/florian.galinier/stibbons\">Website</a></p>"
			"<p><small>"
				"Copyright © 2015 <a href=\"mailto://julia.bassoumi@etud.univ-montp2.fr\">Julia Bassoumi</a><br/>"
				"Copyright © 2015 <a href=\"mailto://florian.galinier@etud.univ-montp2.fr\">Florian Galinier</a><br/>"
				"Copyright © 2015 <a href=\"mailto://adrien.plazas@etud.univ-montp2.fr\">Adrien Plazas</a><br/>"
				"Copyright © 2015 <a href=\"mailto://clement.simon@etud.univ-montp2.fr\">Clement Simon</a>"
			"</small></p>"
			"<p><small><a href=\"http://www.umontpellier.fr/\">University of Montpellier</a></small></p>"
		));
}

void Window::updateInterpreterWaitTime(int waitTime) {
	if (runner)
		runner->setWaitTime(pow(10.0, (double) waitTime / 100.0) - 1);
}

void Window::error(QString type, QString what) {
	QMessageBox::critical(this, type, what);
	cerr << type.toStdString() << ": " << what.toStdString() << endl;

	updateToolbar();
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

void Window::updateToolbar() {
	if (runner) {
		runAct->setVisible(!runner->isRunning());
		haltAct->setVisible(runner->isRunning());
	}
	else {
		runAct->setVisible(false);
		haltAct->setVisible(false);
	}
}

void Window::createTab(QScrollArea *t){
	QString prog;
	prog = (tab == nullptr)?"":textArea->document()->toPlainText();
	tab = new QTabWidget(this);
	print = new QWidget();
	code = new QWidget();

	QVBoxLayout *editor = new QVBoxLayout;
	textArea=new StibbonsEditor;
	highlighter = new StibbonsHighlighter(textArea->document());
	editor->addWidget(textArea);
	textArea->setPlainText(prog);
	code->setLayout(editor);

	QVBoxLayout *stibPrint = new QVBoxLayout;
	stibPrint->addWidget(t);
	print->setLayout(stibPrint);

	tab->addTab(print, "Print");
	tab->addTab(code, "Code");

	this->setCentralWidget(tab); 

}

void Window::nextTab() {
	if (!tab)
		return;

	auto i = tab->count();
	if (i > 0)
		tab->setCurrentIndex((tab->currentIndex() + 1) % i);
}

void Window::previousTab() {
	if (!tab)
		return;

	auto i = tab->count();
	if (i > 0)
		tab->setCurrentIndex((tab->currentIndex() - 1) % i);
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
