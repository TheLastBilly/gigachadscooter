#pragma once

#include "managers/managers.hpp"

#include "graphics/view.hpp"
#include "graphics/animation.hpp"

namespace gcs
{
	namespace views
	{
		class ui : public graphics::view
		{
		public:
			ui(const std::string &name):
				view(name)
			{}

			void setup() override;
			void update(size_t delta) override;
			void cleannup() override;

			inline engine::core* getCore()
			{ return this->core; }

		protected:
			void initialize() override;

		private:
			engine::core* core = nullptr;

			engine::managers::animationsManager* animationsManager = nullptr;
			engine::managers::viewsManager* viewsManager = nullptr;
		};
	}
}