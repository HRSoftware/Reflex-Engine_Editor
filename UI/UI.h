#pragma once
#include <iostream>

#include "Reflex-Core/GameObject.h"
#include "Reflex-Core/Cache.h"
#include "Reflex-AssetManager.h"


namespace UI {

    inline void selectList(std::shared_ptr<Reflex::Core::Cache<Reflex::Core::Model>> models)
    {
        int num;

        if (ImGui::TreeNode("Models")) {
            ImGui::BeginChild("Scrolling");
            for (auto model : models->m_cacheMap) {
                if (ImGui::TreeNode(model.second->modelName.c_str())) {
                    ImGui::Columns(2, nullptr, false);
                    if (ImGui::Selectable("Name", false)) {

                    }
                    char name[32];
                    ImGui::LabelText(name, "", model.second->modelName.c_str());
                    ImGui::NextColumn();
                    ImGui::LabelText(name, "", "test");
                    ImGui::NextColumn();


                    ImGui::TreePop();
                }
                ImGui::Columns(1);
            }
            ImGui::EndChild();
            //ImGui::Begin("Textures View");
            //const auto image = Reflex::AssetFactory._resourceCache->m_TextureCache.findResource("defaultTexture");
            //ImGui::Image((void*)(intptr_t)image.get(), ImVec2(1139, 1138));
            //ImGui::End();
            ImGui::TreePop();
        }

    }
    inline imgui_addons::ImGuiFileBrowser file_dialog;
    inline void updateUI(GLFWwindow* _window)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    inline void ShowDemoWindowPopups()
    {
        if (!ImGui::CollapsingHeader("Popups & Modal windows"))
            return;

        // Popups are windows with a few special properties:
        // - They block normal mouse hovering detection outside them. (*)
        // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
        // - Their visibility state (~bool) is held internally by imgui instead of being held by the programmer as we are used to with regular Begin() calls.
        // (*) One can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
        // Those three properties are intimately connected. The library needs to hold their visibility state because it can close popups at any time.

        // Typical use for regular windows:
        //   bool my_tool_is_active = false; if (ImGui::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
        // Typical use for popups:
        //   if (ImGui::Button("Open")) ImGui::OpenPopup("MyPopup"); if (ImGui::BeginPopup("MyPopup") { [...] EndPopup(); }

        // With popups we have to go through a library call (here OpenPopup) to manipulate the visibility state.
        // This may be a bit confusing at first but it should quickly make sense. Follow on the examples below. 

        if (ImGui::TreeNode("Popups")) {
            ImGui::TextWrapped(
                "When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.");

            static int selected_fish = -1;
            const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
            static bool toggles[] = { true, false, false, false, false };

            // Simple selection popup
            // (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
            if (ImGui::Button("Select.."))
                ImGui::OpenPopup("my_select_popup");
            ImGui::SameLine();
            ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
            if (ImGui::BeginPopup("my_select_popup")) {
                ImGui::Text("Aquarium");
                ImGui::Separator();
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    if (ImGui::Selectable(names[i]))
                        selected_fish = i;
                ImGui::EndPopup();
            }

            // Showing a menu with toggles
            if (ImGui::Button("Toggle.."))
                ImGui::OpenPopup("my_toggle_popup");
            if (ImGui::BeginPopup("my_toggle_popup")) {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], "", &toggles[i]);
                if (ImGui::BeginMenu("Sub-menu")) {
                    ImGui::MenuItem("Click me");
                    ImGui::EndMenu();
                }

                ImGui::Separator();
                ImGui::Text("Tooltip here");
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("I am a tooltip over a popup");

                if (ImGui::Button("Stacked Popup"))
                    ImGui::OpenPopup("another popup");
                if (ImGui::BeginPopup("another popup")) {
                    for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                        ImGui::MenuItem(names[i], "", &toggles[i]);
                    if (ImGui::BeginMenu("Sub-menu")) {
                        ImGui::MenuItem("Click me");
                        ImGui::EndMenu();
                    }
                    ImGui::EndPopup();
                }
                ImGui::EndPopup();
            }

            // Call the more complete ShowExampleMenuFile which we use in various places of this demo
            if (ImGui::Button("File Menu.."))
                ImGui::OpenPopup("my_file_popup");
            if (ImGui::BeginPopup("my_file_popup")) {
                // ShowExampleMenuFile();
                ImGui::EndPopup();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Context menus")) {
            // BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
            //    if (IsItemHovered() && IsMouseReleased(0))
            //       OpenPopup(id);
            //    return BeginPopup(id);
            // For more advanced uses you may want to replicate and cuztomize this code. This the comments inside BeginPopupContextItem() implementation.
            static float value = 0.5f;
            ImGui::Text("Value = %.3f (<-- right-click here)", value);
            if (ImGui::BeginPopupContextItem("item context menu")) {
                if (ImGui::Selectable("Set to zero")) value = 0.0f;
                if (ImGui::Selectable("Set to PI")) value = 3.1415f;
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
                ImGui::PopItemWidth();
                ImGui::EndPopup();
            }

            // We can also use OpenPopupOnItemClick() which is the same as BeginPopupContextItem() but without the Begin call.
            // So here we will make it that clicking on the text field with the right mouse button (1) will toggle the visibility of the popup above.
            ImGui::Text("(You can also right-click me to the same popup as above.)");
            ImGui::OpenPopupOnItemClick("item context menu", 1);

            // When used after an item that has an ID (here the Button), we can skip providing an ID to BeginPopupContextItem(). 
            // BeginPopupContextItem() will use the last item ID as the popup ID.
            // In addition here, we want to include your editable label inside the button label. We use the ### operator to override the ID (read FAQ about ID for details)
            static char name[32] = "Label1";
            char buf[64];
            sprintf_s(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
            ImGui::Button(buf);
            if (ImGui::BeginPopupContextItem()) {
                ImGui::Text("Edit name:");
                ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::SameLine();
            ImGui::Text("(<-- right-click here)");

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Modals")) {
            ImGui::TextWrapped(
                "Modal windows are like popups but the user cannot close them by clicking outside the window.");

            if (ImGui::Button("Delete.."))
                ImGui::OpenPopup("Delete?");
            if (ImGui::BeginPopupModal("Delete?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
                ImGui::Separator();

                //static int dummy_i = 0;
                //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Stacked modals.."))
                ImGui::OpenPopup("Stacked 1");
            if (ImGui::BeginPopupModal("Stacked 1")) {
                ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");
                static int item = 1;
                ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
                static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
                ImGui::ColorEdit4("color", color); // This is to test behavior of stacked regular popups over a modal

                if (ImGui::Button("Add another modal.."))
                    ImGui::OpenPopup("Stacked 2");
                if (ImGui::BeginPopupModal("Stacked 2")) {
                    ImGui::Text("Hello from Stacked The Second!");
                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }

                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Menus inside a regular window")) {
            ImGui::TextWrapped(
                "Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
            ImGui::Separator();
            // NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
            // To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
            // would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
            ImGui::PushID("foo");
            ImGui::MenuItem("Menu item", "CTRL+M");
            if (ImGui::BeginMenu("Menu inside a regular window")) {

                ImGui::EndMenu();
            }
            ImGui::PopID();
            ImGui::Separator();
            ImGui::TreePop();
        }
    }

    inline void debugWindow(int fps, double drawTime)
    {
        if (ImGui::Begin("DebugWindow")) {
            ImGui::Text("Current Fps: %d (%.3f)", fps, drawTime);
            ImGui::End();
        }
    }

    inline void showMainMenu()
    {
        bool open = false, save = false, view = false, entities = false, debug = false, import = false, demo = false, userGuide = false;
        if (ImGui::BeginMainMenuBar()) 
        {
            if (ImGui::BeginMenu("File")) 
            {
                if (ImGui::MenuItem("Open..", nullptr))
                    open = true;

                if (ImGui::MenuItem("Import..", nullptr))
                    import = true;

                if (ImGui::MenuItem("Save", nullptr))
                    save = true;

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) 
            {
                if (ImGui::MenuItem("Entities", nullptr))
                    entities = true;

                if (ImGui::MenuItem("Debug", nullptr))
                    debug = true;

                if (ImGui::MenuItem("Demo", nullptr))
                    demo = true;

                if (ImGui::MenuItem("UserGuide", nullptr))
                    userGuide = true;

                if (ImGui::MenuItem("Exit", nullptr))
                    glfwWindowShouldClose(Reflex::Settings::getInstance()->windowHandle);

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        //Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
        if (open)
            ImGui::OpenPopup("Open File");
        if (save)
            ImGui::OpenPopup("Save File");
        if (view)
            ImGui::OpenPopup("View File");
        if (import)
            ImGui::OpenPopup("Import");
        if (entities)
            ImGui::OpenPopup("Entites");
        if (debug)
            ImGui::OpenPopup("Debug");
        if (demo)
            ImGui::ShowDemoWindow(&demo);
        if (userGuide)
            ImGui::ShowUserGuide();

        /* Optional third parameter. Support opening only compressed rar/zip files.
        * Opening any other file will show error, return false and won't close the dialog.
        */
        if (UI::file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".rar,.zip,.7z")) {
            std::cout << UI::file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
            std::cout << UI::file_dialog.selected_path << std::endl;    // The absolute path to the selected file
        }
        if (UI::file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310))) {
            // const std::string _saveFile = UI::file_dialog.selected_path;
            // Reflex::Log::writeNotice("Saving to {0}", _saveFile + ".yaml");
            // Reflex::Serialisation::Serialise::writeCachesToFile(_saveFile, Reflex::resourceManager._resourceCache, true);
        }
        if (UI::file_dialog.showFileDialog("Import", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".fbx,.obj,.3ds,.FBX")) {

            // Reflex::Log::writeNotice("Importing {0} from {1}", UI::file_dialog.selected_fn, UI::file_dialog.selected_path);;    // The absolute path to the selected file
             //auto model = Reflex::globalAssetFactory.createModel(UI::file_dialog.selected_fn, UI::file_dialog.selected_path);
        }

    }

    inline void menuBar()
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File", true)) {
                if (ImGui::MenuItem("Test Item")) {
                    ImGui::ShowStyleEditor();
                }
                if (ImGui::MenuItem("Open")) {
                    ImGui::OpenPopup("Open File");
                    imgui_addons::ImGuiFileBrowser fileBrowser;
                    if (fileBrowser.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, { 512, 512 })) {
                        std::cout << UI::file_dialog.selected_fn << std::endl;
                        std::cout << UI::file_dialog.selected_path << std::endl;
                        std::cout << UI::file_dialog.ext << std::endl;
                    }
                }
                ImGui::EndMenu();
            };
            ImGui::EndMainMenuBar();
        }
    }

    inline void showTexture(std::shared_ptr<Reflex::Core::Texture> texture)
    {
        if (texture) {
            if (ImGui::Begin("Textures Display")) {

                ImGui::Text("Textures = %s", texture->m_name.c_str());
                ImGui::Image((void*)nullptr, ImVec2(texture->m_dimensions.x, texture->m_dimensions.y));

                ImGui::End();
            }
        }

    }


}

namespace ImGui {
    inline auto vector_getter = [](void* vec, int idx, const char** out_text) {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

    inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }
}