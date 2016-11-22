#include "all_headers.h"


GLProgram::GLProgram(void)
{
	m_ID = glCreateProgram();
}

GLProgram::~GLProgram(void)
{
	glDeleteProgram(m_ID);
}

void GLProgram::AttachShader(GLShader* shader)
{
	glAttachShader(m_ID, shader->GetID());
}

void GLProgram::DetachShader(GLShader* shader)
{
	glDetachShader(m_ID, shader->GetID());
}

bool GLProgram::Link()
{
	glLinkProgram(m_ID);

	int param;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &param);
	if (param == GL_TRUE) return true;
	else return false;
}