#include <SysLib/Ui.hpp>
#include <WinApi.hpp>

using namespace Aiva;
using namespace WinApi;
using namespace SysLib;

void Ui::Loop()
{
    auto msg = MSG{};

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
