#pragma once

#include <array>

#include <QMatrix4x4>

#include "util/abstractexercise.h"

class PolygonalDrawable;

class Exercise62 : public AbstractExercise
{
public:
    Exercise62();
    ~Exercise62() override;

protected:
	bool hasAnimation() override;
    bool initialize(Camera &camera) override;
	void update() override;
    void render(const Camera &camera) override;
	const QString hints() const override;

    QScopedPointer<QOpenGLShaderProgram> m_program;
    PolygonalDrawable* m_drawable;
	int m_animationFrame;
};
