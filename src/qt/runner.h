/**
 * \file runner.h
 * \brief Run Stibbons code in its own thread
 * \author Adrien Plazas
 * \version 0.2
 * \date 25/03/2015
 */

#pragma once

#include <QThread>

#include <string>

#include "../model/turtle.h"

namespace stibbons {

class Runner : public QThread {
	public:
		Runner(Turtle& turtle, std::string& program);
		void run();

	private:
		Turtle& turtle;
		std::string& program;
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
