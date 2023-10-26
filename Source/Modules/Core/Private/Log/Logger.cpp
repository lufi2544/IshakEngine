#include "Log/Logger.h"

// IMGUI
#include "imgui.h"

namespace ishak {

	void Logger::Log(const String& message, ELoggerVerbosity verb)
	{
		String finalLog;

		// TODO Add struct for logs with color and stuff.
		if(verb == ELoggerVerbosity::Warining)
		{

		}else if(verb == ELoggerVerbosity::Error)
		{

		}
		else 
		{

		}

		m_allLogs.Add(message);
	}

	void Logger::Draw()
	{		
		if(!ImGui::Begin("Logs"))
		{
			ImGui::End();
			return;
		}

		// Main window
		/*
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");*/
		/*
			ImGui::SameLine();
			Filter.Draw("Filter", -100.0f);
		*/

		ImGui::Separator();

		if(ImGui::BeginChild("scrolling"), ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)
		{

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			
			ImGuiListClipper clipper;
			clipper.Begin(m_allLogs.Size());

			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{				
					ImGui::TextUnformatted(m_allLogs[line_no].begin(), m_allLogs[line_no].end());
				}
			}
			clipper.End();
			
		}
		ImGui::PopStyleVar();

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);


		ImGui::EndChild();
		ImGui::End();		
	}

}// ishak