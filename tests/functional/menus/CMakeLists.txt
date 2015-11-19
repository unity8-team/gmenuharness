
add_library(
    menu-main SHARED
    MenuMain.cpp
)

target_link_libraries(
    menu-main
    ${GMENU_HARNESS}
)

function(add_menu NAME)
    add_executable(
        ${NAME}
        ${NAME}.cpp
    )
    
    target_link_libraries(
        ${NAME}
        menu-main
    )
endfunction()

add_menu(Simple)
add_menu(Deeper)
