#include "all_headers.h"

class GLProgram
{
public:
	GLProgram(void);
	~GLProgram(void);
	void	AttachShader(GLShader* shader);
	void	DetachShader(GLShader* shader);
	bool	Link();

	void			Use() { glUseProgram(m_ID); }
	GLuint			GetID(){ return m_ID; }

protected:
	GLuint m_ID;
};
