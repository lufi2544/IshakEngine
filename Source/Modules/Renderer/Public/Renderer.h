#pragma once
#include "RendererConfig.h"
#include "CoreMinimal.h"
#include "SDL.h"
#include "SDL/SDL_image.h"
#include "RendererContext.h"

#include "EntityId.h"

#include "Debug/DebugWindow.h"

namespace ishak {

	class Window;
}


namespace ishak {

	/*======================================================
	* Global Renderer class for the engine.
	* The renderer will have a window associated for the rendering
	* pass.
	*======================================================*/
	class RENDERER_API Renderer
	{
		typedef SDL_Renderer* RendererT;
	public:		

		static Renderer& Get();

		void Init();
		void AddRenderingTarget(Window* window);
				
		void Render();		
		void EndFrame();

		RendererT GetSDLRenderer()
		{
			return m_rendererWindowPair.second;
		}

		void ShutDown();
		void QueueRenderCommand(const RendererCommand& command);		

	private:
		Renderer() = default;	

		

		void PreRender();
		void SubmitRendererCommand(const RendererCommand& command);
		void PostSetRenderingTarget(Window* window);
		void ClearFrameRendererCommands();
		void PostRender();

		DebugWindow dWindow;
			
	private:
		static Renderer* singleton;
		std::pair<Window*, RendererT> m_rendererWindowPair;

		
		// TODO TMAP!!!
		std::multimap<uint8, std::pair<Ecs::EntityId, RendererCommand>>m_frameCommandsQueue;			
		TArray<Ecs::EntityId> commandedEntities;
	};

}// ishak
