#ifndef ICOMMAND_H
#define ICOMMAND_H

enum ICT{
	ICTnone,
	ICTnextvar,
	ICTpreviousvar,
	ICTupvar,
	ICTdownvar
};

class ICommand
{
	public :
	
	ICT type;
	
	ICommand()
	{
		type = ICTnone;
	}
	
	~ICommand()
	{
	
	}
};

class ICommandNEXTVAR : public ICommand
{
	public :
	
	ICommandNEXTVAR() : ICommand()
	{
		this->type = ICTnextvar;
	}
	
	~ICommandNEXTVAR()
	{
	
	}
};


class ICommandPREVIOUSVAR : public ICommand
{
	public :
	
	ICommandPREVIOUSVAR() : ICommand()
	{
		this->type = ICTpreviousvar;
	}
	
	~ICommandPREVIOUSVAR()
	{
	
	}
};


class ICommandUPVAR : public ICommand
{
	public :
	
	ICommandUPVAR() : ICommand()
	{
		this->type = ICTupvar;
	}
	
	~ICommandUPVAR()
	{
	
	}
};

class ICommandDOWNVAR : public ICommand
{
	public :
	
	ICommandDOWNVAR() : ICommand()
	{
		this->type = ICTdownvar;
	}
	
	~ICommandDOWNVAR()
	{
	
	}
};



#endif
