#include "PBD_PlaneCloth.h"
#include "GL\glut.h"

PBD_PlaneCloth::PBD_PlaneCloth()
{
}

PBD_PlaneCloth::PBD_PlaneCloth(int width, int height)
{
	m_Res[0] = width;
	m_Res[1] = height;
	int size = m_Res[0] * m_Res[1];
	
	m_Pos.resize(size);
	m_NewPos.resize(size);
	m_Vel.resize(size);
	m_InvMass.resize(size);

	Init();
}

PBD_PlaneCloth::~PBD_PlaneCloth()
{
}

void PBD_PlaneCloth::Init(void)
{
	for (int i = 0; i < m_Res[0]; i++) {
		for (int j = 0; j < m_Res[1]; j++) {
			int index = j * m_Res[0] + i;
			m_InvMass[index] = 1.0;
			m_Pos[index].Set(2.0 * i / (double)m_Res[0], 0.0, 2.0 * j / (double)m_Res[1]);
		}
	}
}

void PBD_PlaneCloth::Simulation(double dt)
{
	Vec3<double> gravity(0.0, -9.8, 0.0);
	double damping = 0.99;
	double horiStrutRestLength = 2.0 / (double)m_Res[0];
	double vertStrutRestLength = 2.0 / (double)m_Res[1];

	// Apply external force
	for (int i = 0; i < m_Res[0]; i++) {
		for (int j = 0; j < m_Res[1]; j++) {
			int index = j * m_Res[0] + i;
			m_Vel[index] += gravity * dt *  m_InvMass[index];
			m_Vel[index] *= damping;
			m_NewPos[index] = m_Pos[index] + (m_Vel[index] * dt);
		}
	}

	// Constraint projection
	int iter = 5;
	for (int k = 0; k < iter; k++) {
		// Vertical and horizontal structural springs
		for (int i = 0; i < m_Res[1]; i++) {
			for (int j = 0; j < m_Res[0] - 1; j++) {
				int index0 = j * m_Res[0] + i;
				int index1 = (j+1) * m_Res[0] + i;
				double c_p1p2 = (m_NewPos[index0] - m_NewPos[index1]).Length() - horiStrutRestLength;
				Vec3<double> dp1 = (m_NewPos[index0] - m_NewPos[index1]);
				Vec3<double> dp2 = (m_NewPos[index0] - m_NewPos[index1]);
				dp1.Normalize();
				dp2.Normalize();
				dp1 *= -m_InvMass[index0] / (m_InvMass[index0] + m_InvMass[index1]) * c_p1p2;
				dp2 *= m_InvMass[index1] / (m_InvMass[index0] + m_InvMass[index1]) * c_p1p2;
				m_NewPos[index0] += dp1;
				m_NewPos[index1] += dp2;
			}
		}
		for (int i = 0; i < m_Res[1] - 1; i++) {
			for (int j = 0; j < m_Res[0]; j++) {
				int index0 = j * m_Res[0] + i;
				int index1 = j * m_Res[0] + (i + 1);
				float c_p1p2 = (m_NewPos[index0] - m_NewPos[index1]).Length() - vertStrutRestLength;
				Vec3<double> dp1 = m_NewPos[index0] - m_NewPos[index1];
				Vec3<double> dp2 = m_NewPos[index0] - m_NewPos[index1];
				dp1.Normalize();
				dp2.Normalize();
				dp1 *= -m_InvMass[index0] / (m_InvMass[index0] + m_InvMass[index1]) * c_p1p2;
				dp2 *= m_InvMass[index1] / (m_InvMass[index0] + m_InvMass[index1]) * c_p1p2;
				m_NewPos[index0] += dp1;
				m_NewPos[index1] += dp2;
			}
		}		
	}	
	// Integrate
	for (int i = 1; i < m_Res[1]; i++) {
		for (int j = 0; j < m_Res[0]; j++) {
			int index = j * m_Res[0] + i;
			m_Vel[index] = (m_NewPos[index] - m_Pos[index]) / dt;
			m_Pos[index] = m_NewPos[index];
		}
	}
}

void PBD_PlaneCloth::ballCollision(const Vec3<double> center, const double radius)
{
	for (int i = 0; i < m_Res[1]; i++) {
		for (int j = 0; j < m_Res[0] - 1; j++) {
			int index = j * m_Res[0] + i;
			Vec3<double> v = m_Pos[index] - center;
			float l = v.Length();
			if (v.Length() < radius) { // 만약 파티클이 공 안에 있으면
				// 공의 표면으로 파티클을 projection
				v.Normalize();
				m_Pos[index].offsetPos(v * (radius - 1));
			}
		}
	
	}
}

void PBD_PlaneCloth::Draw(void)
{
	glDisable(GL_LIGHTING);
	for (int i = 0; i < m_Res[0] - 1; i++) {
		for (int j = 0; j < m_Res[1] - 1; j++) {
			auto p00 = GetPos(i, j);
			auto p10 = GetPos(i + 1, j);
			auto p11 = GetPos(i + 1, j + 1);
			auto p01 = GetPos(i, j + 1);
			int c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			glColor3f((float)c, (float)c, (float)c);
			glBegin(GL_QUADS);
			glVertex3f(p00.x(), p00.y(), p00.z());
			glVertex3f(p10.x(), p10.y(), p10.z());
			glVertex3f(p11.x(), p11.y(), p11.z());
			glVertex3f(p01.x(), p01.y(), p01.z());
			glEnd();
		}
	}
	DrawOutline();
}

void PBD_PlaneCloth::DrawOutline(void)
{
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < m_Res[0] - 1; i++) {
		auto p0 = GetPos(i, 0);
		auto p1 = GetPos(i + 1, 0);
		auto p2 = GetPos(i, m_Res[1] - 1);
		auto p3 = GetPos(i + 1, m_Res[1] - 1);
		auto p4 = GetPos(0, i);
		auto p5 = GetPos(0, i + 1);
		auto p6 = GetPos(m_Res[1] - 1, i);
		auto p7 = GetPos(m_Res[1] - 1, i + 1);
		glVertex3f(p0.x(), p0.y(), p0.z());
		glVertex3f(p1.x(), p1.y(), p1.z());
		glVertex3f(p2.x(), p2.y(), p2.z());
		glVertex3f(p3.x(), p3.y(), p3.z());
		glVertex3f(p4.x(), p4.y(), p4.z());
		glVertex3f(p5.x(), p5.y(), p5.z());
		glVertex3f(p6.x(), p6.y(), p6.z());
		glVertex3f(p7.x(), p7.y(), p7.z());
	}
	glEnd();
	//glLineWidth(1.0f);
}