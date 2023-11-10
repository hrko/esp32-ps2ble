import pandas as pd


# e.g. "E0 F0 37" → "0xE0, 0xF0, 0x37"
def toHexLiteralList(s):
    return ", ".join([f"0x{c}" for c in s.split(" ")])

none_var = "SC_NONE"
unassigned_var = "SC_UNASSIGNED"
def toScanCode(s):
    if s == "None":
        return none_var
    elif s == "UNASSIGNED":
        return unassigned_var
    else:
        return toHexLiteralList(s)

# Read the data
df = pd.read_csv("hid_ps2_table.tsv", sep="\t", dtype=str, keep_default_na=False)

# Create the code
usage_page = ["01", "07", "0C"]
ps2_set = ["1", "2"]

output = ""
output += "// GENERATED CODE START\n"
output += "// clang-format off\n"
output += "static constexpr auto %s = ScanCode();\n" % none_var
output += "static constexpr auto %s = ScanCode();\n" % unassigned_var

for usage in usage_page:
    for set in ps2_set:
        # Filter the data by usage page
        df2 = df[df["HID Usage Page"] == usage]

        # Filter the data by key name
        # If key name is 'RESERVED', 'DO NOT USE' or 'UNASSIGNED', remove the row
        df2 = df2[df2["Key Name"] != "RESERVED"]
        df2 = df2[df2["Key Name"] != "DO NOT USE"]
        df2 = df2[df2["Key Name"] != "UNASSIGNED"]
        # Since 'Break (Ctrl-Pause)' has the same HID Usage ID as 'Pause', remove the row
        # Maybe almost all keyboards don't have Pause/Break key, so it's not a problem
        df2 = df2[df2["Key Name"] != "Break (Ctrl-Pause)"]

        # Create the code
        cpp = ""
        cpp += "// Usage Page: %s, PS/2 Set %s\n" % (usage, set)

        # generate ScanCode
        cpp_scan_code = ""
        cpp_map = "constexpr auto PAGE_%s_SC%s_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({\n" % (usage, set)
        for index, row in df2.iterrows():
            id = row["HID Usage ID"]
            mk = toScanCode(row[f"PS/2 Set {set} Make"])
            brk = toScanCode(row[f"PS/2 Set {set} Break"])
            mk_var = "PAGE_%s_ID_%s_SC%s_MK" % (usage, id, set)
            brk_var = "PAGE_%s_ID_%s_SC%s_BRK" % (usage, id, set)
            pair_var = "PAGE_%s_ID_%s_SC%s_PAIR" % (usage, id, set)

            if mk != "SC_NONE" and mk != "SC_UNASSIGNED":
                cpp_scan_code += "constexpr auto %s = ScanCode{%s};\n" % (mk_var, mk)
            else:
                mk_var = mk

            if brk != "SC_NONE" and brk != "SC_UNASSIGNED":
                cpp_scan_code += "constexpr auto %s = ScanCode{%s};\n" % (brk_var, brk)
            else:
                brk_var = brk

            cpp_scan_code += "constexpr auto %s = ScanCodePair{&%s, &%s};\n" % (pair_var, mk_var, brk_var)
            cpp_map += "  {0x%s, &%s},\n" % (id, pair_var)
        cpp_map += "});\n"
        cpp += cpp_scan_code
        cpp += cpp_map
        output += cpp

output += "// clang-format on\n"
output += "// GENERATED CODE END\n"

# Write the code
with open("hid_ps2_table.cpp", "w") as f:
    f.write(output)