macro(list_subdirectories subdirs dir)

    file(GLOB children RELATIVE ${dir} ${dir}/*)
    set(dirlist "")

    foreach(child ${children})
        if(IS_DIRECTORY ${dir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()

    set(${subdirs} ${dirlist})
    
endmacro()

macro(list_subdirectories_recursive return_list dir)

    file(GLOB_RECURSE new_list ${dir}/*)
    set(dir_list "")

    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        set(dir_list ${dir_list} ${dir_path})
    endforeach()

    list(REMOVE_DUPLICATES dir_list)

    set(${return_list} ${dir_list})

endmacro()

macro(target_include_directories_recursive target dir)
    list_subdirectories_recursive(include_dirs ${dir})
    target_include_directories(${target} PRIVATE ${include_dirs})
endmacro()