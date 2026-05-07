import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def read_text(relative_path):
    return (ROOT / relative_path).read_text(encoding="utf-8")


def profile_block(catalog, profile_id):
    marker = f"{{ProfileId::{profile_id},"
    return [line for line in catalog.splitlines() if marker in line]


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

    def test_project_uses_custom_esp32s3_board_profile(self):
        readme = read_text("README.md")
        workflow = read_text(".github/workflows/repo-smoke.yml")
        board = read_text("hardware/boards/boards.local.txt")

        fqbn = "esp32:esp32:robotic_arm_s3n16r8"
        self.assertIn(fqbn, readme)
        self.assertIn(fqbn, workflow)
        self.assertIn("robotic_arm_s3n16r8.name=IMU Robotic Arm", board)
        self.assertIn("robotic_arm_s3n16r8.build.partitions=app3M_fat9M_16MB", board)
        self.assertNotIn("arduino-cli compile --fqbn esp32:esp32:esp32s3", readme)
        self.assertNotIn("arduino-cli compile --fqbn esp32:esp32:esp32s3", workflow)

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
            "var activeCategory='';",
            "var activeProfile='';",
            "class='remote-card' hidden data-profile",
            "class='brand-card'",
            "data-profile-index",
            "overflow-x:hidden",
            "scrollbar-width:thin",
            "::-webkit-scrollbar",
            ".filters{display:grid",
            "padding-bottom:128px",
            "border:2px solid",
            "appendRemoteLayout",
            "appendDpad",
            "appendRockerControls",
            "appendUtilityControls",
            "class='remote-face'",
            "class='dpad'",
            "grid-template-areas",
            "area-up",
            "class='rocker-grid'",
            "class='utility-grid'",
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
            ".filters{display:flex;gap:8px;overflow:auto",
        ):
            self.assertNotIn(removed, dashboard)

    def test_dashboard_click_handlers_load_before_visible_controls(self):
        dashboard = read_text(
            "firmware/esp32_s3_universal_ir_remote/web_dashboard/WebDashboard.h"
        )

        script_call = dashboard.index("appendDashboardScript(html);")
        body_start = dashboard.index("</head><body")

        self.assertLess(script_call, body_start)

    def test_dashboard_script_uses_browser_safe_node_iteration(self):
        dashboard = read_text(
            "firmware/esp32_s3_universal_ir_remote/web_dashboard/WebDashboard.h"
        )

        self.assertIn("function forEachMatch(selector,callback)", dashboard)
        self.assertNotIn(".forEach(function", dashboard)

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

    def test_catalog_contains_researched_tv_code_expansions(self):
        catalog = read_text(
            "firmware/esp32_s3_universal_ir_remote/ir_catalog/IrCatalog.h"
        )
        research = read_text("docs/IR_CODE_RESEARCH.md")

        expected_commands = {
            "PanasonicTv": (
                "Vol +",
                "Vol -",
                "Mute",
                "Ch +",
                "Ch -",
                "Input",
                "Menu",
                "Up",
                "Down",
                "Left",
                "Right",
                "Enter",
                "HDMI 1",
            ),
            "PhilipsTv": (
                "Vol +",
                "Vol -",
                "Mute",
                "Ch +",
                "Ch -",
                "Input",
                "Menu",
                "Up",
                "Down",
                "Left",
                "Right",
            ),
            "Sharp": (
                "Vol +",
                "Vol -",
                "Mute",
                "Ch +",
                "Ch -",
                "Input",
                "Menu",
                "Info",
            ),
        }

        for profile_id, command_names in expected_commands.items():
            block = "\n".join(profile_block(catalog, profile_id))
            for command_name in command_names:
                self.assertIn(f'"{command_name}"', block)

        self.assertIn("panasonicIrdb", catalog)
        self.assertIn("rc5Irdb", catalog)
        self.assertIn("sharpIrdb", catalog)
        self.assertIn(
            'ProfileId::PhilipsTv, "Power", DeviceCategory::Tv, IrProtocol::Rc5,\n'
            "     CodeStatus::Confirmed",
            catalog,
        )
        self.assertIn("TCL/Hisense", research)
        self.assertIn("RCA-38", research)
        self.assertIn("noch nicht vom Sender unterstuetzt", research)

    def test_philips_first_test_note_records_only_confirmed_power(self):
        note = read_text("docs/session_notes/2026-05-08_philips_tv_first_test.md")

        self.assertIn("Philips TV | Power", note)
        self.assertIn("funktioniert", note)
        self.assertIn("Input | RC5", note)
        self.assertIn("gleiche TV-Reaktion wie `Menu`", note)
        self.assertIn("Nur `Power` wird im aktiven Katalog auf `confirmed` gesetzt.", note)


if __name__ == "__main__":
    unittest.main()
