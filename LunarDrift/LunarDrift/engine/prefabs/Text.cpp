#include "Text.h"
#include "../managers/ShaderManager.h"
#include "../managers/TextureManager.h"
#include "../containers/Model.h"
#include <sstream>


Text::Text(
    Context* context,
    std::weak_ptr<MetaManager> managers,
    glm::vec2& pos,
    const std::string& text,
	bool updateInDraw) 
    : SceneObject(context, managers),
	  m_NeedsUpdating(text.length() > 0),
	  m_UpdateInDraw(updateInDraw)
{
	setPosition(pos);
    setText(text);

    m_Alphabet = std::make_unique<Texture>(context, "gui/alphabet.dds");

    reserveBuffer(context);
}

Text::~Text()
{

}


void Text::step(double dt)
{
}

void Text::predraw(Context* context)
{
	commitUpdates(context);
}

void Text::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
    assert(context);
    assert(shaders.get());

	if (m_UpdateInDraw)
		commitUpdates(context);

    std::shared_ptr<ShaderProgram> shader = shaders->get("text");
    updateModelMatrixState(context, shader);

    m_Alphabet->bind(context);

    m_VAO->bind(context, shader);
    glDrawArrays(
        static_cast<GLenum>(PrimitiveMode::Triangles),
        0,
        m_VAO->getVertexBuffer()->size());
}

void Text::setPosition(const glm::vec2& pos)
{
	SceneObject::setPosition(glm::vec3(pos.x, pos.y, 0.f));
}

void Text::setText(const std::string& text)
{
	m_Text = text;
	transformText();
	m_NeedsUpdating = true;
}

void Text::setNumerical(int number)
{
	std::ostringstream ss("");
	ss << number;
	setText(ss.str());
}

void Text::setNumerical(double number)
{
    std::ostringstream ss("");
    ss << number;
    setText(ss.str());
}

const std::string& Text::getText() const
{
	return m_Text;
}

void Text::addChar(char ch)
{
	m_Text.push_back(ch);
	transformText();
	m_NeedsUpdating = true;
}

void Text::pop()
{
	m_Text.pop_back();
	transformText();
	m_NeedsUpdating = true;
}

void Text::addNumerical(int number)
{
	std::ostringstream ss("");
	ss << number;
	m_Text += ss.str();
	transformText();
	m_NeedsUpdating = true;
}

void Text::addNumerical(double number)
{
    std::ostringstream ss("");
    ss << number;
    m_Text += ss.str();
    transformText();
    m_NeedsUpdating = true;
}

void Text::reserveBuffer(Context* context)
{
    m_Letters.resize(m_MaxTextLength);
    for (Letter& letter : m_Letters)
        memset((void*)letter.vertices, 0, sizeof(VERTEX) * 6);

    std::vector<VERTEX> vertices;
    for (Letter& letter : m_Letters)
        for (unsigned int vertex = 0; vertex < 6; ++vertex)
            vertices.push_back(letter.vertices[vertex]);

    std::vector<VertexAttribute> attribs;
    getLayoutAttribs(&attribs);

    std::shared_ptr<VertexBufferObjectBase> vbo =
        std::make_shared<VertexBufferObject<VERTEX>>(context, vertices, false);
    m_VAO = std::make_unique<VertexArrayObject>(context, vbo, attribs);
}

void Text::commitUpdates(Context* context)
{
	if (!m_NeedsUpdating)
		return;

	plantTextInBuffer();

    std::vector<VERTEX> vertices;
    for (Letter& letter : m_Letters)
        for (unsigned int vertex = 0; vertex < 6; ++vertex)
            vertices.push_back(letter.vertices[vertex]);

    std::shared_ptr<VertexBufferObject<VERTEX>> vbo = 
        std::static_pointer_cast<VertexBufferObject<VERTEX>>(
            m_VAO->getVertexBuffer());
    vbo->setVertices(vertices);
    vbo->resynchronize(context);

	m_NeedsUpdating = false;
}

void Text::plantTextInBuffer() 
{
    for (Letter& letter : m_Letters)
        memset((void*)letter.vertices, 0, sizeof(VERTEX) * 6);

	float xOffset = 0.f;
	const float heightPx = 16.f;
	const float texHeight = 16.f, texWidth = 360.f;
	const float texBottom = 1.f;
	for (unsigned int index = 0; index < m_Text.length(); ++index)
	{
		VERTEX* letter = m_Letters[index].vertices;
		glm::vec2 dims = getLetterTexDimensions(m_Text.at(index));

		float letterWidthPx = (dims.y - dims.x);
		float widthPx = xOffset + (dims.y - dims.x);
		float texLeft = dims.x / texWidth;
		float texRight = dims.y / texWidth;

		letter[0] = VERTEX{ glm::vec2(xOffset, 0.f), glm::vec2(texLeft, 0.f) };		    // Top Left
        letter[1] = VERTEX{ glm::vec2(widthPx, 0.f), glm::vec2(texRight, 0.f) };		// Top Right
		letter[4] = VERTEX{ glm::vec2(xOffset, heightPx), glm::vec2(texLeft, 1.f) };	// Bottom Left
		letter[2] = VERTEX{ glm::vec2(widthPx, heightPx), glm::vec2(texRight, 1.f) };	// Bottom Right
		letter[3] = VERTEX{ glm::vec2(widthPx, heightPx), glm::vec2(texRight, 1.f) };	// Bottom Right
        letter[5] = VERTEX{ glm::vec2(xOffset, 0.f), glm::vec2(texLeft, 0.f) };		    // Top Left

		xOffset += letterWidthPx;
	}
}

unsigned int Text::getTextWidth(const std::string& text)
{
	unsigned int width = 0;
	for (char letter : text)
	{
		const glm::vec2& dims = getLetterTexDimensions(letter);
		width += static_cast<unsigned int>(dims.y - dims.x);
	}
	return width;
}

glm::vec2 Text::getLetterTexDimensions(char letter)
{
	switch (letter)
	{
		case '0':
		{
			return glm::vec2(0, 10);
		}
		case '1':
		{
            return glm::vec2(10, 19);
		}
		case '2':
		{
            return glm::vec2(19, 30);
		}
		case '3':
		{
            return glm::vec2(30, 39);
		}
		case '4':
		{
            return glm::vec2(39, 49);
		}
		case '5':
		{
            return glm::vec2(49, 59);
		}
		case '6':
		{
            return glm::vec2(59, 68);
		}
		case '7':
		{
            return glm::vec2(68, 78);
		}
		case '8':
		{
            return glm::vec2(78, 88);
		}
		case '9':
		{
            return glm::vec2(88, 98);
		}
		case ' ':
		{
            return glm::vec2(98, 108);
		}
		case 'A':
		{
            return glm::vec2(108, 117);
		}
		case 'B':
		{
            return glm::vec2(117, 127);
		}
		case 'C':
		{
            return glm::vec2(127, 137);
		}
		case 'D':
		{
            return glm::vec2(137, 147);
		}
		case 'E':
		{
            return glm::vec2(147, 156);
		}
		case 'F':
		{
            return glm::vec2(156, 166);
		}
		case 'G':
		{
            return glm::vec2(166, 176);
		}
		case 'H':
		{
            return glm::vec2(176, 186);
		}
		case 'I':
		{
            return glm::vec2(186, 196);
		}
		case 'J':
		{
            return glm::vec2(196, 205);
		}
		case 'K':
		{
            return glm::vec2(205, 215);
		}
		case 'L':
		{
            return glm::vec2(215, 225);
		}
		case 'M':
		{
            return glm::vec2(225, 235);
		}
		case 'N':
		{
            return glm::vec2(235, 244);
		}
		case 'O':
		{
            return glm::vec2(244, 254);
		}
		case 'P':
		{
            return glm::vec2(254, 264);
		}
		case 'Q':
		{
            return glm::vec2(264, 274);
		}
		case 'R':
		{
            return glm::vec2(274, 284);
		}
		case 'S':
		{
            return glm::vec2(284, 294);
		}
		case 'T':
		{
            return glm::vec2(294, 303);
		}
		case 'U':
		{
            return glm::vec2(303, 313);
		}
		case 'V':
		{
            return glm::vec2(313, 323);
		}
		case 'W':
		{
            return glm::vec2(323, 333);
		}
		case 'X':
		{
            return glm::vec2(333, 342);
		}
		case 'Y':
		{
            return glm::vec2(342, 352);
		}
		case 'Z':
		{
            return glm::vec2(352, 360);
		}
		default:
		{
			return getLetterTexDimensions(' ');
		}
	}
}

void Text::transformText()
{
	for (char& letter : m_Text)
	{
		if (!isalnum(letter))
		{
			letter = ' ';
		}
		else 
		{
			letter = toupper(letter);
		}
	}
}

void Text::getLayoutAttribs(std::vector<VertexAttribute>* attribs) const
{
    attribs->push_back(VertexAttribute(
        "Position",
        offsetof(VERTEX, position),
        sizeof(VERTEX),
        VertexAttribute::Type::Float,
        2));
    attribs->push_back(VertexAttribute(
        "TexCoord",
        offsetof(VERTEX, texCoord),
        sizeof(VERTEX),
        VertexAttribute::Type::Float,
        2));
}
