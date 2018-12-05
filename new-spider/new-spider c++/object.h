#pragma once
class object
{
public:
	object();
	~object();

	int hashCode(const char chars[]);
private:
	int hash;
};

object::object()
{
}

object::~object()
{
}

int object::hashCode(const char chars[])
{
	hash = 0;
	int length = sizeof(chars);
	if (hash == 0 && length > 0)
	{
		for (int i = 0; i < length; i++)
		{
			hash = (hash << 5) - hash + chars[i];
		}
	}
	return hash;
}