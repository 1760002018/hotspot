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

#define SSID L"ChrisEric1 Virtual Machine Box"
#define PASS L"CE1VMBOX"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Windows.Networking.NetworkOperators.h>
#include <winrt/Windows.Devices.WiFiDirect.h>
#include <winrt/Windows.Security.Credentials.h>

int main() {
	wprintf(L"\n    ChrisEric1 Virtual Machine Box (CE1VMBOX)\n    Copyright (C) 2007-2025, Christopher Eric Lentocha\n\n    This program is free software: you can redistribute it and/or modify\n    it under the terms of the GNU Affero General Public License as published\n    by the Free Software Foundation, either version 3 of the License, or\n    (at your option) any later version.\n\n    This program is distributed in the hope that it will be useful,\n    but WITHOUT ANY WARRANTY; without even the implied warranty of\n    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n    GNU Affero General Public License for more details.\n\n    You should have received a copy of the GNU Affero General Public License\n    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n");
	winrt::init_apartment();
	auto connectionProfile{ 
		winrt::Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile()
	};
	auto tetheringManager = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
	auto credential = winrt::Windows::Security::Credentials::PasswordCredential();
	credential.Password(PASS);
	auto conf = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringAccessPointConfiguration();
	tetheringManager.StopTetheringAsync();
	tetheringManager.DisableNoConnectionsTimeoutAsync();
	conf.Band(winrt::Windows::Networking::NetworkOperators::TetheringWiFiBand::TwoPointFourGigahertz);
	conf.Ssid(SSID);
	conf.Passphrase(credential.Password());
	tetheringManager.ConfigureAccessPointAsync(conf);
	while (true) {
		if (tetheringManager.TetheringOperationalState() == winrt::Windows::Networking::NetworkOperators::TetheringOperationalState::Off) {
			tetheringManager.StartTetheringAsync();
			wprintf(L"\nHotspot Powered On! :\n\n - SSID\n\t%s\n\n - PASS\n\t%s\n", SSID, PASS);
		}
	}
}
