#ifndef VAR_H
#define VAR_H

#include "../EXP/EXP.h"

class VAR : public EXP
{
	/* cannot have any argument at all. */
	protected :
	float value;
		
	public :
	
	int id;
	float& valueREF;
	
	VAR(float val = 0.0f);
	VAR(const VAR& v);
	~VAR();
	
	virtual float evaluate()	override;
	VAR operator=(const VAR& v);
	
	
	void setValue(const float val);
	float getValue()	const	{	return this->value;	}
	
	
};


#endif
