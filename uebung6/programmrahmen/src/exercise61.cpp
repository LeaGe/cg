#include "exercise61.h"

#include <cassert>

#include <QGuiApplication>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QtMath>

#include "util/camera.h"
#include "util/glviewer.h"
#include "util/polygonaldrawable.h"
#include "util/objio.h"

Exercise61::Exercise61()
    : AbstractExercise()
	, m_lateralSurfaces(16)
	, m_wireframeModeEnabled(false)
{
}

Exercise61::~Exercise61()
{
}

bool Exercise61::hasAnimation()
{
	return false;
}

bool Exercise61::initialize(Camera &camera)
{
    AbstractExercise::initialize(camera);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	camera.setCenter(QVector3D(0.0f, 1.0f, 0.0f));
	camera.setEye(QVector3D(0.0f, 2.0f, 6.0f));
	
	m_program.reset(createBasicShaderProgram("data/cone.vert", "data/cone.frag"));
    m_program->bind();

	//TODO: Create VAO & VBOs

	updateCone();

	return true;
}

void Exercise61::render(const Camera &camera)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
	m_program->setUniformValue("normalMatrix", camera.view().normalMatrix());
	m_program->setUniformValue("lightsource", QVector3D(0.0, 10.0, 4.0));
	m_program->bind();

	m_program->setUniformValue("viewprojection", camera.viewProjection());

	if (m_wireframeModeEnabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// TODO: Add call to glDrawArrays
	
	if (m_wireframeModeEnabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

void Exercise61::updateCone()
{
	qDebug() << "Lateral surfaces: " << m_lateralSurfaces;

	assert(m_lateralSurfaces >= 3);

	//TODO: Update buffer content

}

bool Exercise61::onKeyPressed(QKeyEvent *event, Camera &camera)
{
	bool handled = false;

	if (!event->modifiers())
	{
		handled = true;
		
		switch (event->key())
		{
			case Qt::Key_W:
				m_wireframeModeEnabled = !m_wireframeModeEnabled;
				break;
			case '1':
				camera.setCenter(QVector3D(0.0f, 1.0f, 0.0f));
				camera.setEye(QVector3D(0.0f, 2.0f, 6.0f));
				break;
			case '2':
				camera.setCenter(QVector3D(0.0f, 1.0f, 0.0f));
				camera.setEye(QVector3D(0.0f, -5.0f, 0.1f));
				break;
			case Qt::Key_Plus:
				m_lateralSurfaces++;
				updateCone();
				break;
			case Qt::Key_Minus:
				if (m_lateralSurfaces > 3)
				{
					m_lateralSurfaces--;
					updateCone();
				}
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

const QString Exercise61::hints() const
{
	return "Keys:\n\t[+] / [-] : Increase/Decrease the number of lateral surfaces.\n\t[1] / [2] : Switch camera perspective.\n\t[w] : Toggle wireframe mode.";
}

int main(int argc, char *argv[])
{
	// Create application
	QGuiApplication app(argc, argv);

	// Create main window
	GLViewer viewer(new Exercise61);
	viewer.show();

	// Run application
	return app.exec();
}
