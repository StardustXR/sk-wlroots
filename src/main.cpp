#include "stereokit.h"

using namespace sk;

int main() {
	sk_settings_t sk_settings = {};
	sk_settings.app_name = "sk-wlroots demo";
	sk_settings.flatscreen_width = 1600;
	sk_settings.flatscreen_height = 900;
	if (!sk_init(sk_settings))
		return 1;

	while (sk_step([]() {
	}));

	return 0;
}