#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <vector>
#include <map>
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class PageManager {
public:
    using TaskFunction_t = void (*)(void*);
    
    struct TaskConfig {
        TaskHandle_t handle;
        TaskFunction_t taskFunc;
        const char* taskName;
        uint32_t stackDepth;
        UBaseType_t priority;
        BaseType_t coreID;
    };

    static void register_task_page(lv_obj_t* screen, 
                                 TaskFunction_t func,
                                 const char* name,
                                 uint32_t stack ,
                                 UBaseType_t prio ,
                                 BaseType_t core );

    static void handle_page_events(lv_event_t* e);

private:
    static std::map<lv_obj_t*, std::vector<TaskConfig>> taskMap;
};

#endif