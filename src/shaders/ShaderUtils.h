#pragma once
#include <glad/glad.h>
#include <vector>

using namespace std;

class VAO_VBO
{
public:
	unsigned int VBO;
	unsigned int VAO;

	void disponse()
	{
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
	}
};


class ShaderUtils
{
public:
	static VAO_VBO loadOnlyCoordinate(float vertices[], int size)
	{
        VAO_VBO vao_vbo;
		
        glGenVertexArrays(1, &vao_vbo.VAO);
        glGenBuffers(1, &vao_vbo.VBO);

        glBindVertexArray(vao_vbo.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vao_vbo.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
		
        return vao_vbo;
	}

    static VAO_VBO loadTextureAndCoordinate(float vertices[], int size)
    {
        VAO_VBO vao_vbo;

        glGenVertexArrays(1, &vao_vbo.VAO);
        glGenBuffers(1, &vao_vbo.VBO);

        glBindVertexArray(vao_vbo.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vao_vbo.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);

        // ������������ ��������
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
		
        // �������� ���������� ���������
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
		
        return vao_vbo;
    }

    static VAO_VBO load2vec3(float vertices[], int size)
    {
        VAO_VBO vao_vbo;

        glGenVertexArrays(1, &vao_vbo.VAO);
        glGenBuffers(1, &vao_vbo.VBO);

        glBindVertexArray(vao_vbo.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vao_vbo.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        return vao_vbo;
    }

    static VAO_VBO load2vec3And1vec2(float vertices[], int size)
    {
        VAO_VBO vao_vbo;

        glGenVertexArrays(1, &vao_vbo.VAO);
        glGenBuffers(1, &vao_vbo.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, vao_vbo.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);

        glBindVertexArray(vao_vbo.VAO);
		
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
		
        glBindVertexArray(0);

        return vao_vbo;
    }

    static VAO_VBO load2vec2(float vertices[], int size)
    {
        VAO_VBO vao_vbo;
        glGenVertexArrays(1, &vao_vbo.VAO);
        glGenBuffers(1, &vao_vbo.VBO);
        glBindVertexArray(vao_vbo.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, vao_vbo.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        return vao_vbo;
    }

    static VAO_VBO loadvec2(float vertices[], int size)
    {
        VAO_VBO vao_vbo;
        glGenVertexArrays(1, &vao_vbo.VAO);
        glGenBuffers(1, &vao_vbo.VBO);
        glBindVertexArray(vao_vbo.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, vao_vbo.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        return vao_vbo;
    }

    static vector<float> getQuad()
    {
        return vector<float>
        {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };
    }

	static vector<float> getCube()
	{
        return vector<float>
		{
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
        };
	}

	static vector<float> getCubeWithNormal()
	{
        return vector<float> {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
        };
	}

	static vector<float> getCubeWithNormalAndTexture()
	{
        vector<float> vertices = {
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
           -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        return vertices;
	}
	
    static vector<float> getCube(float scaleTexture)
    {
        return vector<float>
        {
            -0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, -0.5f, -0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, 0.5f, -0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, 0.5f, -0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, 0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,

            -0.5f, -0.5f, 0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, -0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, 0.5f, 0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, -0.5f, 0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,

            -0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            -0.5f, 0.5f, -0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            -0.5f, -0.5f, 0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,
            -0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,

            0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, 0.5f, -0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, -0.5f, 0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,

            -0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, -0.5f, -0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, -0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, -0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            -0.5f, -0.5f, 0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,
            -0.5f, -0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,

            -0.5f, 0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, 0.5f, -0.5f, 1.0f * scaleTexture, 1.0f * scaleTexture,
            0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            0.5f, 0.5f, 0.5f, 1.0f * scaleTexture, 0.0f * scaleTexture,
            -0.5f, 0.5f, 0.5f, 0.0f * scaleTexture, 0.0f * scaleTexture,
            -0.5f, 0.5f, -0.5f, 0.0f * scaleTexture, 1.0f * scaleTexture
        };
    }
};