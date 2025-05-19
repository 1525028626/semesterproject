#include "System/PageManager.h"
#include "System/Conf.h"

std::map<lv_obj_t*, std::vector<PageManager::TaskConfig>> PageManager::taskMap;

void PageManager::register_task_page(lv_obj_t* screen, TaskFunction_t func, 
                                   const char* name, uint32_t stack, UBaseType_t prio, BaseType_t core) {
    // 首次注册时添加事件回调
    if(taskMap[screen].empty()) {
        lv_obj_add_event_cb(screen, handle_page_events, LV_EVENT_ALL, NULL);
    }
    // 添加新配置到列表
    taskMap[screen].push_back({nullptr, func, name, stack, prio, core});
}

// 修改事件处理
void PageManager::handle_page_events(lv_event_t* e) {
    lv_obj_t* screen = lv_event_get_target(e);
    
    if(e->code == LV_EVENT_SCREEN_LOADED) {
        for(auto& config : taskMap[screen]) {
            if(config.handle) vTaskDelete(config.handle);
            xTaskCreatePinnedToCore(config.taskFunc, config.taskName, config.stackDepth, 
                                  nullptr, config.priority, &config.handle, config.coreID);
        }
    }
    
    if(e->code == LV_EVENT_SCREEN_UNLOADED) {
        for(auto& config : taskMap[screen]) {
            if(config.handle) {
                vTaskDelete(config.handle);
                config.handle = nullptr;
            }
        }
    }
}