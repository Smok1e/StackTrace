#pragma once

//----------------------------------------------------------------

#define DOT_PATH "C:\\Graphviz\\bin\\dot.exe"

#define CREATE_GRAPH(name) Graph name (#name);

//----------------------------------------------------------------

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

	}

	void Add (const char * format, ...);

	void Render ();

private :

	FILE * file_;

	std::string name_;

};

//----------------------------------------------------------------

void Graph::Add (const char * format, ...)

{

	va_list args;

	va_start (args, format);

	vfprintf (file_, format, args);

	va_end (args);

}

//----------------------------------------------------------------

void Graph::Render ()

{

	Add ("\n}\n");

	char line [512] = "";

	sprintf_s (line, "%s %s -Tpng -o%s.png", DOT_PATH, name_.c_str (), name_.c_str ());

	fclose (file_);

	if (system (line) == 0)

	{

		fopen_s (&file_, name_.c_str (), "a");

		sprintf_s (line, "start %s.png", name_.c_str ());

		system (line);

	}

}
