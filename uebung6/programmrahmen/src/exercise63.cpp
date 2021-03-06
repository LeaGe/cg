#include "exercise63.h"

#include <cassert>

#include <QGuiApplication>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "util/camera.h"
#include "util/glviewer.h"

Exercise63::Exercise63()
    : AbstractExercise()
	, m_animationFrame(0)
	, m_wireframeModeEnabled(false)
	, m_maximumTessGenLevel(2)
{
}

Exercise63::~Exercise63()
{
}

bool Exercise63::hasAnimation()
{
	return true;
}

const QString Exercise63::hints() const
{
	return "Keys:\n\t[SPACE] : Pause/Resume animation.\n\t[w] : Toggle wireframe mode.";
}

bool Exercise63::onKeyPressed(QKeyEvent *event, Camera &camera)
{
    bool handled = false;

	if(!event->modifiers())
	{
		handled = true;
		
		switch(event->key())
		{
		case Qt::Key_W:
			m_wireframeModeEnabled = !m_wireframeModeEnabled;
			break;
			
		default:
			handled = false;
			break;
		}

		if (handled)
		{
			requestRedraw();
		}
	}

    return handled || AbstractExercise::onKeyPressed(event, camera);
}

bool Exercise63::initialize(Camera &camera)
{
    AbstractExercise::initialize(camera);

	camera.setCenter(QVector3D(0.0f, 0.0f, 0.0f));
	camera.setEye(QVector3D(-4.0f, 4.0f, 6.0f));

	glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &m_maximumTessGenLevel);
	qDebug().noquote() << "GL_MAX_TESS_GEN_LEVEL == " << m_maximumTessGenLevel;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	m_program.reset(new QOpenGLShaderProgram());
    m_program->create();

	addShader(QOpenGLShader::Vertex, "data/cube.vert", *m_program);
	addShader(QOpenGLShader::Fragment, "data/cube.frag", *m_program);
	
	// TODO: uncomment the following lines to use the
	// tessellation control shader, tessellation evaluation shader
	// and the geometry shader.
	//addShader(QOpenGLShader::TessellationControl, "data/cube.tcs", *m_program);
    //addShader(QOpenGLShader::TessellationEvaluation, "data/cube.tes", *m_program);
    //addShader(QOpenGLShader::Geometry, "data/cube.geom", *m_program);
	
    m_program->link();
    m_program->bind();

    m_vao.create();
    m_vao.bind();

	// TODO: Replace the dummy triangle with the geometry of the cube.

	m_vertices.resize(3);
	int index = -1;

	// dummy triangle
	m_vertices[++index] = QVector3D(-0.5f, -0.5f, 0.5f);
    m_vertices[++index] = QVector3D(0.5f, -0.5f, 0.5f);
    m_vertices[++index] = QVector3D(0.0f, 0.5f, 0.5f);

	m_vertexBuffer = QOpenGLBuffer();
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(QVector3D));
    m_program->setAttributeBuffer("in_vertex", GL_FLOAT, 0, 3, sizeof(QVector3D));
    m_program->enableAttributeArray("in_vertex");

	// TODO: Define here the number of patch vertices by using OpenGL.

	return true;
}

void Exercise63::update()
{
	m_animationFrame++;
	if (m_animationFrame > 1000)
	{
		m_animationFrame = 0;
	}
}

void Exercise63::render(const Camera &camera)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_program->bind();
	m_program->setUniformValue("lightsource", QVector3D(0.0, 10.0, 4.0));
	m_program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
	if (m_animationFrame > 50)
	{
		m_program->setUniformValue("animationFrame", 1.0f + (m_maximumTessGenLevel - 1.0f) * ((m_animationFrame - 50) / 1000.0f));
	}
	else
	{
		m_program->setUniformValue("animationFrame", 1.0f);
	}

	m_program->setUniformValue("view", camera.view());
	m_program->setUniformValue("viewprojection", camera.viewProjection());
	m_program->setUniformValue("transform", QMatrix4x4());
	m_program->setUniformValue("normalMatrix", camera.view().normalMatrix());

	if (m_wireframeModeEnabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

    // draw
	// TODO: Replaces GL_TRIANGLES with the appropriate primitive type
	// for OpenGL tessellation shaders.
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	if (m_wireframeModeEnabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

int main(int argc, char *argv[])
{
	// Create application
	QGuiApplication app(argc, argv);

	// Create main window
	GLViewer viewer(new Exercise63());
	viewer.show();

	// Run application
	return app.exec();
}
