import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def read_text(relative_path):
    return (ROOT / relative_path).read_text(encoding="utf-8")


class WebDashboardDirectionTest(unittest.TestCase):
    def test_sketch_defaults_to_web_dashboard(self):
        sketch = read_text(
            "firmware/esp32_s3_universal_ir_remote/"
            "esp32_s3_universal_ir_remote.ino"
        )

        self.assertIn('#include "web_dashboard/WebDashboard.h"', sketch)
        self.assertIn("web_dashboard::RemoteWebDashboard", sketch)
        self.assertNotIn("ButtonInput", sketch)
        self.assertNotIn("RemoteStateMachine", sketch)
        self.assertNotIn("RemoteDisplay", sketch)

    def test_config_declares_hotspot_defaults(self):
        config = read_text(
            "firmware/esp32_s3_universal_ir_remote/config/Config.h"
        )

        self.assertIn('kWifiApSsid = "IR-Remote"', config)
        self.assertIn('kWifiApPassword = "irremote"', config)
        self.assertIn("kWebServerPort = 80", config)
        self.assertIn('kDashboardIp = "192.168.4.1"', config)

    def test_web_dashboard_module_exposes_expected_routes(self):
        dashboard = read_text(
            "firmware/esp32_s3_universal_ir_remote/web_dashboard/WebDashboard.h"
        )

        for expected in (
            "WiFi.softAP",
            "WebServer",
            "handleRoot",
            "handleSend",
            "handleSweep",
            '"/"',
            '"/send"',
            '"/sweep"',
        ):
            self.assertIn(expected, dashboard)

    def test_dashboard_has_filterable_testing_ui(self):
        dashboard = read_text(
            "firmware/esp32_s3_universal_ir_remote/web_dashboard/WebDashboard.h"
        )

        for expected in (
            "data-profile",
            "data-category",
            "data-command",
            "id='search'",
            "id='profilePicker'",
            "id='emptyState'",
            "filterCategory",
            "selectProfile",
            "sendCommand",
            "fetch(form.action",
            "applyFilters",
            "clearSearch",
            "confirmCommand",
            "return confirm",
            "appendCategoryFilter",
            "appendProfileOption",
            "appendCommandGroup",
            "appendDashboardScript",
            "classifyCommand",
            "let activeCategory='';",
            "let activeProfile='';",
            "class='remote-card' hidden data-profile",
            "class='brand-card'",
            "data-profile-index",
        ):
            self.assertIn(expected, dashboard)

        for removed in (
            "class='stats'",
            "<strong>Profile</strong>",
            "<strong>Befehle</strong>",
            "<strong>Status</strong><span>Test</span>",
            "config::kWifiApSsid;\n    html += F(\" / http://\");",
            "appendCategoryFilter(html, \"all\"",
            "F(\"Alle\")",
            "data-filter='all'",
            "activeCategory='all'",
            "activeCategory==='all'",
            "class='profile-card'",
        ):
            self.assertNotIn(removed, dashboard)

    def test_docs_describe_dashboard_as_default_and_hardware_as_minimal(self):
        readme = read_text("README.md")
        hardware = read_text("hardware/README.md")
        future = read_text("future/README.md")

        self.assertIn("Web-Dashboard", readme)
        self.assertIn("http://192.168.4.1", readme)
        self.assertIn("Taster und LCD sind optional", readme)

        self.assertIn("Minimal-Hardware", hardware)
        self.assertIn("IR-Sendepfad bleibt Pflicht", hardware)
        self.assertIn("Taster", future)
        self.assertIn("LCD1602", future)


if __name__ == "__main__":
    unittest.main()
