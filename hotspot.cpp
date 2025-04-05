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

	//	cl hotspot.cpp /std:c++17 /EHsc /link

#define SSID L"ChrisEric1 Virtual Machine Box"
#define PASS L"CE1VMBOX"

#include <windows.h>
#include <tlhelp32.h>
#include <wininet.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Windows.Networking.NetworkOperators.h>
#include <winrt/Windows.Security.Credentials.h>
#pragma comment(lib, "WindowsApp.lib")
#pragma comment(lib, "wininet.lib")

int KillTheGGProxy() {
	INTERNET_PER_CONN_OPTION_LIST list;
	INTERNET_PER_CONN_OPTION options[2];
	DWORD size = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	list.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	list.pszConnection = NULL;
	list.dwOptionCount = 2;
	list.dwOptionError = 0;
	list.pOptions = options;
	options[0].dwOption = INTERNET_PER_CONN_FLAGS;
	options[0].Value.dwValue = PROXY_TYPE_DIRECT;
	options[1].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	options[1].Value.pszValue = NULL;
	InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, size);
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
	return 0;
}

int KillTheGGApp(wchar_t *processName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(hSnapshot, &pe32)) {
			do {
				if (wcscmp(processName, pe32.szExeFile) == 0) {
					HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
					if (hProcess) {
						if (TerminateProcess(hProcess, 0)) {
							wprintf(L"\nSuccess at Killing \n\t%s\n", processName);
						} else {
							wprintf(L"\nFailed at Killing \n\t%s\n", processName);
						}
						CloseHandle(hProcess);
					} else {
						wprintf(L"\nSpyware Killer Failed!\n");
					}
				}
			} while (Process32NextW(hSnapshot, &pe32));
		}
	} else {
		wprintf(L"\nSpyware Killer Failed!\n");
	}
	CloseHandle(hSnapshot);
	return 0;
}

int HotspotInit() {
	winrt::init_apartment();
	auto connectionProfile = winrt::Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile();
	auto tetheringManager = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
	auto credential = winrt::Windows::Security::Credentials::PasswordCredential();
	credential.Password(PASS);
	auto conf = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringAccessPointConfiguration();
	tetheringManager.StopTetheringAsync();
	conf.Ssid(SSID);
	conf.Passphrase(credential.Password());
	tetheringManager.ConfigureAccessPointAsync(conf);
	try {
		tetheringManager.DisableNoConnectionsTimeoutAsync();
		conf.Band(winrt::Windows::Networking::NetworkOperators::TetheringWiFiBand::TwoPointFourGigahertz);
		tetheringManager.ConfigureAccessPointAsync(conf);
	} catch (...) {
		wprintf(L"\nDisabling No Connection Timeouts and/or Changing Hotspot Wireless Band to 2.4 GHz Failed!\nIs this device running at least Windows 10 Build 19041 or later?\n");
	}
	return 0;
}

int HotspotPwrOn() {
	winrt::init_apartment();
	auto connectionProfile = winrt::Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile();
	auto tetheringManager = winrt::Windows::Networking::NetworkOperators::NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
	if (tetheringManager.TetheringOperationalState() == winrt::Windows::Networking::NetworkOperators::TetheringOperationalState::Off) {
		tetheringManager.StartTetheringAsync();
		wprintf(L"\nHotspot Power On Success! :\n\n - SSID\n\t%s\n\n - PASS\n\t%s\n", SSID, PASS);
	}
	return 0;
}

int TryAllInit() {
	try {
		HotspotInit();
		while (true) {
			try {
				HotspotPwrOn();
				try {
					KillTheGGProxy();
					KillTheGGApp(L"AppMonitor.exe");
					// KillTheGGApp(L"ElevationService.exe");
					KillTheGGApp(L"TheGoGuardianApp.exe");
				} catch (...) {
					wprintf(L"\nSpyware Killer Initialization Failed!\n");
				}
			} catch (...) {
				wprintf(L"\nHotspot Power On Failed!\n");
			}
		}
	} catch (...) {
		wprintf(L"\nHotspot Initialization Failed!\n");
		while (true) {
			try {
				KillTheGGProxy();
				KillTheGGApp(L"AppMonitor.exe");
				// KillTheGGApp(L"ElevationService.exe")
				KillTheGGApp(L"TheGoGuardianApp.exe");;
			} catch (...) {
				wprintf(L"\nSpyware Killer Initialization Failed!\n");;
			}
		}
	}
	return 0;
}

int main() {
	wprintf(L"\n    ChrisEric1 Virtual Machine Box (CE1VMBOX)\n    Copyright (C) 2007-2025, Christopher Eric Lentocha\n\n    This program is free software: you can redistribute it and/or modify\n    it under the terms of the GNU Affero General Public License as published\n    by the Free Software Foundation, either version 3 of the License, or\n    (at your option) any later version.\n\n    This program is distributed in the hope that it will be useful,\n    but WITHOUT ANY WARRANTY; without even the implied warranty of\n    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n    GNU Affero General Public License for more details.\n\n    You should have received a copy of the GNU Affero General Public License\n    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n");
	try {
		TryAllInit();
	} catch (...) {
		wprintf(L"\nInitialization Failed!\n");
	}
	return 0;
}
