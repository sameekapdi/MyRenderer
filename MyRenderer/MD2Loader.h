#pragma once

// forward  declaration
namespace renderer { class Object;}

class MD2Loader
{
public:
	MD2Loader(void); //default constructor
public:
	~MD2Loader(void); //destructor

public:
	static bool LoadModel(const char* pFilename, renderer::Object&);
};

