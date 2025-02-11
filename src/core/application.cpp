#include <core/application.hpp>

static const uint32_t WIDTH = 800;
static const uint32_t HEIGHT = 600;

namespace oys {

	Application::Application() {
		m_window = Window{ WIDTH, HEIGHT, "Oyster Engine" };
		m_vulkanInstance = VulkanInstance{};
		m_window.createSurface(m_vulkanInstance.getInstance(), &m_surface);
		VulkanDevice m_vulkanDevice{ m_vulkanInstance.getInstance(), m_surface };
		Swapchain m_swapchain{ &m_vulkanDevice, m_surface, m_window.getExtent() };

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);
		Pipeline m_pipeline{ m_vulkanDevice, pipelineConfig };
	}

	Application::~Application() {
		cleanup();
	}

	void Application::init() {
		
	}

	void Application::update() {
		m_window.poll();
	}

	void Application::cleanup() {
		m_pipeline.cleanup();
		m_swapchain.cleanup();
		m_window.cleanup();
		vkDestroySurfaceKHR(m_vulkanInstance.getInstance(), m_surface, nullptr);
		m_vulkanDevice.cleanup();
		m_vulkanInstance.cleanup();
	}
}