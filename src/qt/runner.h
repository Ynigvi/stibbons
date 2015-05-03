/**
 * \file runner.h
 * \brief Run Stibbons code in its own thread
 * \author Adrien Plazas
 * \version 0.4
 * \date 25/03/2015
 */

#pragma once

#include <QThread>

#include <mutex>
#include <string>

#include "../interpreter/interpreter-manager.h"

namespace stibbons {

class Runner : public QThread {
	Q_OBJECT

	public:
		Runner(std::string& program);
		~Runner();
		WorldPtr getWorld();

		void start();

		void run();
		void halt();

		std::string exportModel();

		bool isRunning();

	signals:
		void error(QString type, QString what);

	public slots:
		/**
		 * Set the wait time used to slow down the interpretations.
		 *
		 * \param waitTime the waited time
		 */
		void setWaitTime(size_t waitTime);

	private:
		void unhalt();

		InterpreterManager* manager;
		bool started;
		bool running;
		std::recursive_mutex haltMutex;
};

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
