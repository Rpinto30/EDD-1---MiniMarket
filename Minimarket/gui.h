#ifndef GUI
#define GUI

// NO SOY UN EXPERTO EN WINAPI, pero solo lo hago para que se vea bonito
// ME GUIE DE LA DOCUMENTACINO DE WINDOWS: https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
namespace window{
    #include <windows.h>

    class Window{
        private:
            WNDCLASSEX wc = {};
            HWND hwnd = NULL;
            /* CALLBACK GENERAL PARA RECIBIR LOS MENSAJES DE LA VENTANA*/
            static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
                switch (msg) {
                    case WM_CLOSE:
                        DestroyWindow(hwnd);
                        break;
                    case WM_DESTROY:
                        PostQuitMessage(0);
                        break;
                    default:
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                }
                return 0;
            }

            int registerWindow(){
                wc.cbSize = sizeof(WNDCLASSEX);
                wc.lpfnWndProc = WndProc;
                wc.hInstance = GetModuleHandle(NULL);

                wc.lpszClassName = "PRUEBA";
                if(!RegisterClassEx(&wc)){
                    MessageBox(NULL, "Algo fallo pendejo", "Error", MB_ICONERROR | MB_OK);
                    return -1;
                }
                return 0;
            }

            int createHandle(){
                /*CREACION HANDLE */
                hwnd = CreateWindowEx(
                    0, // estilo extendido
                    "PRUEBA",
                    "Tu mama se me hace una persona muy amable",
                    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // estilo fijo
                    CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,// x,y Width, height
                    NULL, //PARENT
                    NULL, //Menu de arriba
                    GetModuleHandle(NULL),
                    NULL //data (ni idea pero asi lo pide xd)
                );

                if (hwnd == NULL) return -1;
                return 0;
            }
        public:
            Window(){
                if(registerWindow() == -1) return;
                if(createHandle() == -1) return;

                ShowWindowAsync(hwnd, SW_SHOWNORMAL);
                UpdateWindow(hwnd);
            }

            int run(){
                MSG msg = {};
                while (GetMessage(&msg, NULL, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                return static_cast<int>(msg.wParam);
            }

            int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
                Window win;
                return win.run();
            }

    };
}

#endif // GUI

