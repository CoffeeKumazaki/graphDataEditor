#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "load_image.hpp"
#include "struct.hpp"
#include "menu_widget.hpp"
#include "editor.hpp"


int main(int argc, char const *argv[]) {

	std::string img_path;
	if (argc > 1) {
		img_path = argv[1];
	}

	int w = 1280;
	int h = 720;
	std::string title = "Graph Data Editor";
	initImgui(w, h, title);

	GraphDataEditor editor;
	editor.init();
	NODE_PRT wn; 	// working node 
	NODE_PRT fn; 	// focus node

	// Our state
	ImVec4 clear_color = ImVec4(89.0f/255.0, 88.0f/255.0, 87.0f/255.0, 1.00f);
	ImVec4 node_color = ImVec4(230.0/255.0, 180.0/255.0f, 34.0f/255.0, 1.00f);
	ImVec4 edge_color = ImVec4(255.0/255.0, 255.0/255.0f, 255.0f/255.0, 1.00f);

	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	if (!img_path.empty()) {
		bool ret = LoadTextureFromFile(img_path.c_str(), &my_image_texture, &my_image_width, &my_image_height);
	}

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// menu
		static int state = 0;
		static float node_size = 5.0;
		bool chstate = false;
		{
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoBackground;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(w/4, 720), ImGuiCond_FirstUseEver);

			ImGui::Begin("menu", nullptr, window_flags);
			ImDrawList *draw_list = ImGui::GetWindowDrawList();

			ImGui::Text("Configuration");

			ImGui::SliderFloat("Node size", &node_size, 5.0f, 15.0f);
			for (size_t i = 0; i < STATE_NUM; i++) {
				chstate |= ImGui::RadioButton(state_char[i].c_str(), &state, i);
			}

			if (ImGui::Button("Output")) {
				editor.output();
			}
			if (ImGui::Button("Reset")) {
				editor.init();
			}

			ImGui::End();
		}

		// main
		{
			ImGuiIO &io = ImGui::GetIO();
			if (chstate || io.MouseClicked[1])
				wn = NULL;
			ImVec2 mouse_pos = ImGui::GetMousePos();
			fn = editor.getHoverNode(mouse_pos.x, mouse_pos.y, node_size);
			ImGui::GetMouseDragDelta();
			ImVec2 click_pos = io.MouseClickedPos[0];

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			// window_flags |= ImGuiWindowFlags_NoBackground;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::SetNextWindowPos(ImVec2(w/4, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(3.0*w/4, 720), ImGuiCond_FirstUseEver);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(82.0f/255.0f, 78.0f/255.0f, 77.0f/255.0f, 1.0f)); // Set window background to red

			ImGui::Begin("main", nullptr, window_flags);
			ImDrawList *draw_list = ImGui::GetWindowDrawList();

			ImGui::Text("Node Num: %d", editor.numNode());

			if (my_image_texture)
				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(3.0*w/4, h));
	
			if (state == NOTHING) {
			}
			else {
				if (io.MouseClicked[0] && mouse_pos.x > w/4 && mouse_pos.x < w && mouse_pos.y > 0 && mouse_pos.y < h) {

					if (state == DELETE) {
						editor.delNode(fn);
					}
					else if (state == ONLY_EDGE) {
						if (wn && fn) {
							wn->child.push_back(fn);
							fn->child.push_back(wn);
						}
						else if (fn) {
							wn = fn;
						}
					}
					else {
						if (wn && fn) {
							wn->child.push_back(fn);
							fn->child.push_back(wn);
							wn = fn;
						}
						else if (fn) {
							wn = fn;
						}
						else {
							auto n = std::make_shared<Node>();
							n->x = click_pos.x;
							n->y = click_pos.y;
							if (wn && state == NODE_W_EDGE) {
								n->child.push_back(wn);
								wn->child.push_back(n);
							}
							editor.addNode(n);
							wn = n;
						}
					}
				}
			}

			for (auto it = editor.nodeBegin(), itEnd = editor.nodeEnd(); it != itEnd; ++it) {
				auto node = (*it);
				draw_list->AddCircle(ImVec2(node->x, node->y), node_size, ImColor(node_color));
				for (auto cit = node->child.begin(), citEnd = node->child.end(); cit != citEnd; ++cit){
					auto ch = (*cit);
					draw_list->AddLine(ImVec2(node->x, node->y), ImVec2(ch->x, ch->y), ImColor(edge_color));
				}
			}

			if (fn) {
				draw_list->AddCircleFilled(ImVec2(fn->x, fn->y), node_size, ImColor(node_color), 100);
			}

			if (wn && state != ONLY_NODE) {
				draw_list->AddLine(ImVec2(wn->x, wn->y), mouse_pos, ImColor(edge_color));
			}

			ImGui::PopStyleColor();
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
