#pragma once

#include "oys_window.hpp"
#include "oys_pipeline.hpp"
#include "oys_device.hpp"

namespace oys {
	class FirstApp {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		OysWindow oysWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		OysDevice oysDevice{ oysWindow };
		OysPipeline oysPipeline{
			oysDevice,
			"../shaders/simple_shader.vert.spv",
			"../shaders/simple_shader.frag.spv",
			OysPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
	};
} // namespace oys