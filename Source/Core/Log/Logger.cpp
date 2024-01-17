#include "Core/Log/Logger.h"

// IMGUI
#include "IMGUI/imgui.h"

namespace ishak {
	

	void Logger::Log(const String& message, ELoggerVerbosity verb)
	{
		String finalLog;

		// TODO Add struct for logs with color and stuff.
		if(m_allLogs.Size() > 5000)
		{
			return;
		}

		m_allLogs.Add({ verb, message });
	}

	void Logger::Draw()
	{		
		if(!ImGui::Begin("Logs"))
		{
			ImGui::End();
			return;
		}

		// Main window
		
		bool clear = ImGui::Button("Clear");
		if(clear)
		{
			m_allLogs.Clear();
		}
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

					ImVec4 imColor;
					bool bColor{ false };
					LogData& logData{ m_allLogs[line_no] };
					if(logData.verb == ELoggerVerbosity::Warning)
					{

						imColor = ImVec4(255.0f, 255.0f, 0.0f, 1.0f);
						bColor = true;

					}else if(logData.verb == ELoggerVerbosity::Error)
					{
						imColor = ImVec4(255.0f, 0.0f, 0.0f, 1.0f);
						bColor = true;
					}

					if(bColor)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, imColor);
					}

					ImGui::TextUnformatted(m_allLogs[line_no].msg.begin(), m_allLogs[line_no].msg.end());

					if (bColor)
					{
						ImGui::PopStyleColor();
					}
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


	CORE_API Logger* GetGlobalLogger()
	{
		static Logger sLogger;

		return &sLogger;
	}


}// ishak
