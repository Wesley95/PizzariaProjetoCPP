#pragma once

class TipoEstoque
{
public:
	const char* path;

private:
	const char* name;
	const char* level;
	const char* measure;
	const char* typeproduct;
	char key;

public:
	TipoEstoque(const char* initpath, const char* initmeasurement, const char* initname, const char* initlevel, const char* producttype);
	void changeestoque();
	void inaddestoque();
	void removeestoque();
	int fillconsoleestoqueselect(const char* tittle);
	void fillconsoleestoque(const char* tittle);
	bool itemexistence(const char* str, const char startc, const char endc);
	bool unittype(char* unit);
};