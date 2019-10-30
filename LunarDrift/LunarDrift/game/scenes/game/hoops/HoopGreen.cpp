#include"HoopGreen.h"
#include "../../../../engine/comms/MessageSystem.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/components/CollisionComponent.h"
#include "../../../../engine/comms/predefined/CollisionMessage.h"
#include "../../../../engine/comms/predefined/ScoreAddMessage.h"

const float boostRate = 10.5f;

HoopGreen::HoopGreen(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position,
	const glm::vec2& rotation)
	: SceneObject(context, managers)
{
	setPosition(position);
	setRotations(glm::vec3(rotation.x, rotation.y, 0.f));

	m_Model = std::make_shared<Model>(context, "Hoop");
	m_Texture = std::make_unique<Texture>(context, "HoopGreen.dds");
	m_TextureAlt = std::make_unique<Texture>(context, "Grey.dds");
	crossed = false;

	MessageSystem::getInstance().subscribe("Collision", this);
	std::vector<CollisionSphere> points;
	// Middle
	points.push_back({ 1, glm::vec3(0, 2.8, 0), 0.35 });
	points.push_back({ 2, glm::vec3(0, 2.0, 0), 0.35 });
	points.push_back({ 3, glm::vec3(0, 1.2, 0), 0.35 });
	points.push_back({ 4, glm::vec3(0, 0.4, 0), 0.35 });
	points.push_back({ 5, glm::vec3(0, -0.6, 0), 0.35 });
	points.push_back({ 6, glm::vec3(0, -1.4, 0), 0.35 });
	points.push_back({ 7, glm::vec3(0, -2.2, 0), 0.35 });
	points.push_back({ 8, glm::vec3(0, -3.0, 0), 0.35 });
	// Left
	points.push_back({ 9, glm::vec3(0.0, 2.6, -0.8), 0.35 });
	points.push_back({ 10, glm::vec3(0.0, 1.8, -0.8), 0.35 });
	points.push_back({ 11, glm::vec3(0.0, 1.0, -0.8), 0.35 });
	points.push_back({ 12, glm::vec3(0.0, 0.2, -0.8), 0.35 });
	points.push_back({ 13, glm::vec3(0.0, -0.4, -0.8), 0.35 });
	points.push_back({ 14, glm::vec3(0.0, -1.2, -0.8), 0.35 });
	points.push_back({ 15, glm::vec3(0.0, -2.0, -0.8), 0.35 });
	points.push_back({ 16, glm::vec3(0.0, -2.8, -0.8), 0.35 });
	// Left left
	points.push_back({ 17, glm::vec3(0.0, 2.2, -1.6), 0.35 });
	points.push_back({ 18, glm::vec3(0.0, 1.4, -1.6), 0.35 });
	points.push_back({ 19, glm::vec3(0.0, 0.6, -1.6), 0.35 });
	points.push_back({ 20, glm::vec3(0.0, -0.2, -1.6), 0.35 });
	points.push_back({ 21, glm::vec3(0.0, -1.0, -1.6), 0.35 });
	points.push_back({ 22, glm::vec3(0.0, -1.8, -1.6), 0.35 });
	// Left left left
	points.push_back({ 23, glm::vec3(0.0, 0.8, -2.4), 0.35 });
	points.push_back({ 24, glm::vec3(0.0, 0.0, -2.4), 0.35 });
	points.push_back({ 25, glm::vec3(0.0, -0.8, -2.4), 0.35 });
	// Right
	points.push_back({ 9, glm::vec3(0.0, 2.6, 0.8), 0.35 });
	points.push_back({ 10, glm::vec3(0.0, 1.8, 0.8), 0.35 });
	points.push_back({ 11, glm::vec3(0.0, 1.0, 0.8), 0.35 });
	points.push_back({ 12, glm::vec3(0.0, 0.2, 0.8), 0.35 });
	points.push_back({ 13, glm::vec3(0.0, -0.4, 0.8), 0.35 });
	points.push_back({ 14, glm::vec3(0.0, -1.2, 0.8), 0.35 });
	points.push_back({ 15, glm::vec3(0.0, -2.0, 0.8), 0.35 });
	points.push_back({ 16, glm::vec3(0.0, -2.8, 0.8), 0.35 });
	// Right Right
	points.push_back({ 17, glm::vec3(0.0, 2.2, 1.6), 0.35 });
	points.push_back({ 18, glm::vec3(0.0, 1.4, 1.6), 0.35 });
	points.push_back({ 19, glm::vec3(0.0, 0.6, 1.6), 0.35 });
	points.push_back({ 20, glm::vec3(0.0, -0.2, 1.6), 0.35 });
	points.push_back({ 21, glm::vec3(0.0, -1.0, 1.6), 0.35 });
	points.push_back({ 22, glm::vec3(0.0, -1.8, 1.6), 0.35 });
	// Right right right
	points.push_back({ 23, glm::vec3(0.0, 0.8, 2.4), 0.35 });
	points.push_back({ 24, glm::vec3(0.0, 0.0, 2.4), 0.35 });
	points.push_back({ 25, glm::vec3(0.0, -0.8, 2.4), 0.35 });

	addComponent(std::make_shared<CollisionComponent>(this, managers, 3.0f, points));
}

HoopGreen::~HoopGreen()
{
	MessageSystem::getInstance().unsubscribe("Collision", this);
}

void HoopGreen::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void HoopGreen::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	if (!crossed)
		m_Texture->bind(context);
	else
		m_TextureAlt->bind(context);
	m_Model->draw(context, shader);
}

void HoopGreen::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("Collision") == 0)
	{
		std::shared_ptr<CollisionMessage> dMsg =
			std::static_pointer_cast<CollisionMessage>(event);

		SceneObject* so = dMsg->getPartner();
		Ship* temp = dynamic_cast<Ship*>(so);	// magic happens.
		if (temp != nullptr)
		{
			if (!crossed)
			{
				std::shared_ptr<PhysicsComponent> pc =
					std::static_pointer_cast<PhysicsComponent>(temp->getComponent("physics"));

				pc->setVelocity(pc->getVelocity() * 2.0f);

				std::shared_ptr<ScoreAddMessage> sam =
					std::make_shared<ScoreAddMessage>(temp->isPlayer2Controlled(), 1);
				MessageSystem::getInstance().broadcast(sam);

				crossed = true;
			}
		}
	}
} 