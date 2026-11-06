"""Plots for DFT32 array-partition sweep (Q4 b/c). Values come from the csynth reports."""
import matplotlib
import matplotlib.ticker
matplotlib.use("Agg")  # remove this line if you want plt.show()
import matplotlib.pyplot as plt

SURFACE = "#fcfcfb"
INK = "#0b0b0b"
INK2 = "#52514e"
GRID = "#e4e3df"
BLUE, ORANGE, AQUA, YELLOW = "#2a78d6", "#eb6834", "#1baf7a", "#eda100"

factors = [1, 2, 4, 8, 16, 32]
labels = [str(f) for f in factors]
x = list(range(len(factors)))

latency = [221, 213, 209, 207, 206, 203]
interval = [222, 214, 210, 208, 207, 32]
throughput = [1e8 / ii for ii in interval]  # DFT/s at 10 ns clock

bram = [64, 64, 64, 64, 64, 64]
dsp = [640] * 6
ff = [59189, 59314, 59054, 58540, 57515, 61509]
lut = [91521, 91675, 91669, 91705, 91252, 93250]
AVAIL = {"BRAM": 280, "DSP": 220, "FF": 106400, "LUT": 53200}

plt.rcParams.update({
    "font.size": 11,
    "axes.edgecolor": GRID,
    "axes.labelcolor": INK2,
    "xtick.color": INK2,
    "ytick.color": INK2,
    "text.color": INK,
})


def style(ax, xlabel, ylabel, title):
    ax.set_facecolor(SURFACE)
    ax.figure.set_facecolor(SURFACE)
    ax.grid(axis="y", color=GRID, linewidth=1)
    ax.set_axisbelow(True)
    for s in ("top", "right"):
        ax.spines[s].set_visible(False)
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title, loc="left", fontsize=13, fontweight="bold", color=INK)


# ---- (b) resource utilization (all four on one plot) -------------------------
pct = {
    "BRAM_18K": [v / AVAIL["BRAM"] * 100 for v in bram],
    "DSP": [v / AVAIL["DSP"] * 100 for v in dsp],
    "FF": [v / AVAIL["FF"] * 100 for v in ff],
    "LUT": [v / AVAIL["LUT"] * 100 for v in lut],
}
colors = {"BRAM_18K": BLUE, "DSP": ORANGE, "FF": AQUA, "LUT": YELLOW}

fig, ax = plt.subplots(figsize=(8.5, 5))
style(ax, "Array partition factor", "Utilization of xc7z020 (%)",
      "Resource utilization vs. partition factor")
ax.set_title("Resource utilization vs. partition factor", loc="left", fontsize=13,
             fontweight="bold", color=INK, pad=28)
for name, ys in pct.items():
    ax.plot(x, ys, color=colors[name], linewidth=2, marker="o", markersize=8,
            markeredgecolor=SURFACE, markeredgewidth=2, label=name)
ax.axhline(100, color=INK2, linewidth=1, linestyle=(0, (4, 3)))
ax.text(-0.15, 103, "100% of device", color=INK2, fontsize=9, ha="left")
ax.set_ylim(0, 340)
ax.set_xlim(-0.3, len(x) - 1 + 0.6)

# direct labels at the right end (offset to avoid collisions)
for name, ys in pct.items():
    ax.annotate(f"{name}  {int(ys[-1])}%", (x[-1], ys[-1]),
                xytext=(10, 0), textcoords="offset points", va="center",
                fontsize=9, color=INK2)
ax.legend(frameon=False, loc="lower left", ncol=4, bbox_to_anchor=(0, 1.0),
          fontsize=10)
fig.tight_layout()
fig.savefig("resource_vs_partition.png", dpi=200)
plt.close(fig)

# ---- (c1) throughput ---------------------------------------------------------
fig, ax = plt.subplots(figsize=(8.5, 5))
style(ax, "Array partition factor", "Throughput (DFT/s)",
      "Throughput vs. partition factor")
ax.plot(x, throughput, color=BLUE, linewidth=2, marker="o", markersize=8,
        markeredgecolor=SURFACE, markeredgewidth=2)
for xi, t in zip(x, throughput):
    below = xi < len(x) - 1
    ax.annotate(f"{t:,.0f}", (xi, t), xytext=(0, -20 if below else 10),
                textcoords="offset points", ha="center", fontsize=9, color=INK2)
ax.set_ylim(0, max(throughput) * 1.15)
ax.yaxis.set_major_formatter(matplotlib.ticker.FuncFormatter(lambda v, _: f"{v:,.0f}"))
fig.tight_layout()
fig.savefig("throughput_vs_partition.png", dpi=200)
plt.close(fig)

# ---- (c2) latency ------------------------------------------------------------
fig, ax = plt.subplots(figsize=(8.5, 5))
style(ax, "Array partition factor", "Latency (clock cycles)",
      "Latency vs. partition factor")
ax.plot(x, latency, color=ORANGE, linewidth=2, marker="o", markersize=8,
        markeredgecolor=SURFACE, markeredgewidth=2)
for xi, l in zip(x, latency):
    ax.annotate(f"{l}", (xi, l), xytext=(0, 10), textcoords="offset points",
                ha="center", fontsize=9, color=INK2)
ax.set_ylim(195, 227)
ax.text(0.99, 0.02, "y-axis starts at 195", transform=ax.transAxes, ha="right",
        fontsize=8, color=INK2)
fig.tight_layout()
fig.savefig("latency_vs_partition.png", dpi=200)
plt.close(fig)

