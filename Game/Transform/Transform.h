#pragma once

/**
 * トランスフォームクラス
 */
class Transform
{
private:
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;


public:
	Vector3& GetPosition()
	{
		return m_position;
	}


	Quaternion& GetRotation()
	{
		return m_rotation;
	}


	Vector3& GetScale()
	{
		return m_scale;
	}


	void SetPosition(Vector3 position)
	{
		m_position = position;
	}


	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}


	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}


};

