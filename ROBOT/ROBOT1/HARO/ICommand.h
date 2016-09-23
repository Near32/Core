#ifndef ICOMMAND_H
#define ICOMMAND_H

enum ICT{
	ICTnone,
	ICTnextvar,
	ICTpreviousvar,
	ICTupvar,
	ICTdownvar
};

enum SIDE{
	SIDEnone,
	SIDEright,
	SIDEleft
};
	

class ICommand
{
	public :
	
	ICT type;
	SIDE side; 
	
	ICommand(SIDE side_ = SIDEright) : side(side_)
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
	
	ICommandNEXTVAR(SIDE side_ = SIDEright) : ICommand(side_)
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
	
	ICommandPREVIOUSVAR(SIDE side_ = SIDEright) : ICommand(side_)
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
	
	ICommandUPVAR(SIDE side_ = SIDEright) : ICommand(side_)
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
	
	ICommandDOWNVAR(SIDE side_ = SIDEright) : ICommand(side_)
	{
		this->type = ICTdownvar;
	}
	
	~ICommandDOWNVAR()
	{
	
	}
};



#endif
