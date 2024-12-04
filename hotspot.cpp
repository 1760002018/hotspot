/*

    ChrisEric1 Virtual Machine Box (CE1VMBOX)
    Copyright (C) 2007-2025, Christopher Eric Lentocha

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

	//	cl hotspot.cpp /std:c++17 /EHsc /link /DEFAULTLIB:WindowsApp.lib

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Windows.Networking.NetworkOperators.h>
#include <winrt/Windows.Devices.WiFiDirect.h>
#include <winrt/Windows.Security.Credentials.h>

int main() {
      printf("\n    ChrisEric1 Virtual Machine Box (CE1VMBOX)\n    Copyright (C) 2007-2025, Christopher Eric Lentocha\n\n    This program is free software: you can redistribute it and/or modify\n    it under the terms of the GNU Affero General Public License as published\n    by the Free Software Foundation, either version 3 of the License, or\n    (at your option) any later version.\n\n    This program is distributed in the hope that it will be useful,\n    but WITHOUT ANY WARRANTY; without even the implied warranty of\n    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n    GNU Affero General Public License for more details.\n\n    You should have received a copy of the GNU Affero General Public License\n    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n");
      winrt::init_apartment();
      auto connectionProfile{ winrt::Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile() };
      auto tetheringManager = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
      auto credential = winrt::Windows::Security::Credentials::PasswordCredential(); credential.Password(L"CE1VMBOX");
      auto conf = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringAccessPointConfiguration();
      conf.Ssid(L"ChrisEric1 Virtual Machine Box"); conf.Passphrase(credential.Password());
      auto oldConf = tetheringManager.GetCurrentAccessPointConfiguration();
      auto oldSsid = oldConf.Ssid(); auto oldPwd = oldConf.Passphrase();
      tetheringManager.ConfigureAccessPointAsync(conf);
      auto ioAsync = tetheringManager.StartTetheringAsync();
      auto fResult = ioAsync.get();
      winrt::clear_factory_cache();
      winrt::uninit_apartment();
      printf("Hotspot Powered On!\n");
      return 0;
}
