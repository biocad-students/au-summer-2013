__author__ = 'mactep'

AMINO_COLORS = {}


class AAColors(object):
    def __init__(self):
        self.light_green = (0x77, 0xdd, 0x88)
        self.green = (0x99, 0xee, 0x66)
        self.dark_green = (0x55, 0xbb, 0x33)
        self.blue = (0x66, 0xbb, 0xff)
        self.lilac = (0x99, 0x99, 0xff)
        self.dark_blue = (0x55, 0x55, 0xff)
        self.orange = (0xff, 0xcc, 0x77)
        self.pink = (0xee, 0xaa, 0xaa)
        self.red = (0xff, 0x44, 0x55)
        self.gray = (0x22, 0x22, 0x22)

ac = AAColors()

AMINO_COLORS["A"] = ac.light_green
AMINO_COLORS["G"] = ac.light_green

AMINO_COLORS["C"] = ac.green

AMINO_COLORS["D"] = ac.dark_green
AMINO_COLORS["E"] = ac.dark_green
AMINO_COLORS["N"] = ac.dark_green
AMINO_COLORS["Q"] = ac.dark_green

AMINO_COLORS["I"] = ac.blue
AMINO_COLORS["L"] = ac.blue
AMINO_COLORS["M"] = ac.blue
AMINO_COLORS["V"] = ac.blue

AMINO_COLORS["F"] = ac.lilac
AMINO_COLORS["W"] = ac.lilac
AMINO_COLORS["Y"] = ac.lilac

AMINO_COLORS["H"] = ac.dark_blue

AMINO_COLORS["K"] = ac.orange
AMINO_COLORS["R"] = ac.orange

AMINO_COLORS["P"] = ac.pink

AMINO_COLORS["S"] = ac.red
AMINO_COLORS["T"] = ac.red

AMINO_COLORS["-"] = ac.gray