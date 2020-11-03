#ifndef FLOOR_SHADOW_H
#define FLOOR_SHADOW_H

#include <stdint.h>

#include <glm/glm.hpp>

namespace CGRA350 {

class FloorShadow
{
public:
	static FloorShadow* getInstance();

	virtual void renderGUI();

	void render();

	void renderShadow();

	void shadowDebug();

protected:
	FloorShadow();
	virtual ~FloorShadow();
	FloorShadow(const FloorShadow&);
	FloorShadow& operator = (const FloorShadow&);

	static FloorShadow* _instance;

private:
	uint32_t _depthMapFBO;
	uint32_t _depthMap;

	bool _renderShadow;
	bool _shadowDebug;


};

} // namespace CGRA350

#endif // FLOOR_SHADOW_H