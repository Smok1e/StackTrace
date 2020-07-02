#pragma once

#include "C:\Lib\ExtraUtilits\ExtraUtilits.h"

#define DOT_PATH "C:\\Graphviz\\bin\\dot.exe"

#define CREATE_GRAPH(name) Graph name (#name);

class Graph 

{

public :

	Graph (std::string name) :

		name_ (name)

	{

		fopen_s (&file_, name.c_str (), "w");

		assert (file_);

		Add ("digraph G\n{\n\n");
		Add ("edge [fontcolor = green4]");

	}

	~Graph ()

	{
	
		fclose (file_);

		file_ = nullptr;

		//_unlink (name_.c_str ());

	}

	void Add (const char * format, ...);

	void Render ();

private :

	FILE * file_;

	std::string name_;

};

size_t _initGraph ();

void RenderGraph (std::string descr);

void ShowGraph ();

size_t _GraphInitResult = _initGraph ();

void Graph::Add (const char * format, ...)

{

	va_list args;

	va_start (args, format);

	vfprintf (file_, format, args);

	va_end (args);

}

void Graph::Render ()

{

	Add ("\n}\n");

	char line [_EU_BUFFSIZE] = "";

	sprintf_s (line, "%s %s -Tpng -o%s.png", DOT_PATH, name_.c_str (), name_.c_str ());

	fclose (file_);

	if (system (line) == 0)

	{

		fopen_s (&file_, name_.c_str (), "a");

		sprintf_s (line, "start %s.png", name_.c_str ());

		system (line);

	}

}

size_t _initGraph ()

{

	if (!eu::fileExists (DOT_PATH)) 

	{

		MessageBox (NULL, "'" DOT_PATH "' not found", "Graph init error", MB_ICONERROR | MB_OK);

		abort ();

	}

	return 0;

}

void RenderGraph (std::string descr)

{

	FILE * file = nullptr;

	fopen_s (&file, "descr.dot", "w");

	assert (file);

	fprintf_s (file, descr.c_str ());

	fclose (file);

	file = nullptr;

	system (DOT_PATH " descr.dot -Tpng -ograph.png");

}

void ShowGraph ()

{

	assert (eu::fileExists ("graph.png"));

	system ("start graph.png");

}