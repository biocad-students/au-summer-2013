import parse


def _regionsortrule(s):
	reglist = [i + str(j) for j in range(5) for i in ["FR", "CDR"]]
	return reglist.index(s)


def _parseheaderhelper(arg, header):
	try:
		ig_arg = parse.search("%s={:w}" % arg, header).fixed[0]
	except:
		ig_arg = "Unknown"
	return ig_arg


class FIGA(object):
	ig_name = "Unknown"
	ig_type = "Unknown"
	ig_alpha = "Unknown"
	ig_specie = "Unknown"
	ig_seq = ""
	ig_regions = {}
	ig_additional = {}

	def setname(self, newname):
		self.ig_name = newname

	def getname(self):
		return self.ig_name

	def settype(self, newtype):
		self.ig_type = newtype

	def gettype(self):
		return self.ig_type

	def setalpha(self, newalpha):
		self.ig_alpha = newalpha

	def getalpha(self):
		return self.ig_alpha

	def setspecie(self, newspecie):
		self.ig_specie = newspecie

	def getspecie(self):
		return self.ig_specie

	def setseq(self, newseq):
		self.ig_seq = newseq

	def getseq(self):
		return self.ig_seq

	def setregion(self, regionname, region):
		self.ig_regions[regionname] = region

	def getregion(self, regionname):
		return self.ig_regions[regionname]

	def setadditional(self, additionalname, value):
		self.ig_additional[additionalname] = value

	def getadditional(self, additionalname):
		return self.ig_additional[additionalname]

	def serialize(self):
		sorted_regions = sorted(self.ig_regions.items(), key=lambda x: _regionsortrule(x[0]))
		result = [">%s@(type=%s, alpha=%s, specie=%s)" % (self.ig_name, self.ig_type, 
														  self.ig_alpha, self.ig_specie)]
		for region, value in sorted_regions:
			result.append(";@%s: %s" % (region, value))
		for additional, value in self.ig_additional.items():
			result.append(";+%s: %s" % (additional, value))
		result.append(self.ig_seq)
		return "\n".join(result)

	def parseheader(self, header):
		header = "".join(header.split())
		try:
			self.ig_name = parse.search(">{:S}@", header).fixed[0]
		except:
			self.ig_name = "Unknown"
		self.ig_type = _parseheaderhelper("type", header)
		self.ig_alpha = _parseheaderhelper("alpha", header)
		self.ig_specie = _parseheaderhelper("specie", header)
		
	def parseseq(self, seq):
		self.ig_seq = seq

	def parseregion(self, region):
		region = "".join(region.split())
		try:
			name = parse.search(";@{:w}:", region).fixed[0]
			reg = parse.search(":({:d},{:d})", region).fixed
			self.ig_regions[name] = reg
		except:
			pass

	def parseadditional(self, additional):
		additional = "".join(additional.split())
		try:
			name = parse.search(";+{:w}:", additional).fixed[0]
			add = parse.search(":{:w}", additional).fixed[0]
			self.ig_additional[name] = add
		except:
			pass

	def parse(self, string):
		lines = string.splitlines(True)
		header = lines[0]
		self.parseheader(header)
		for line in lines[1:]:
			if line.startswith(";@"):
				self.parseregion(line)
			elif line.startswith(";+"):
				self.parseadditional(line)
			else:
				self.parseseq(line)


def test_generate_figa():
	a = FIGA()
	a.setname("test-figa")
	a.settype("VH")
	a.setspecie("llama")
	a.setalpha("amino")
	a.setseq("EVQLVQSGAEVKKPGESLKISCKGSGYSFTTYWLGWVRQMPGKGLDWIGIMSPVDSDIRYSPSFQGQVTMSV"
			 "DKSITTAYLQWNSLKASDTAMYYCARRRPGQGYFDFWGQGTLVTVSSASTKGPSVFPLA")
	a.setregion("FR1", (0, 29))
	a.setregion("CDR1", (30, 34))
	a.setregion("FR2", (35, 48))
	a.setregion("CDR2", (49, 65))
	a.setregion("FR3", (66, 95))
	a.setregion("CDR3", (96, 107))
	a.setregion("FR4", (108, len(a.getseq()) - 1))
	return a.serialize()


def test_load_figa(s):
	a = FIGA()
	a.parse(s)
	return a


def test():
	s = test_generate_figa()
	t = test_load_figa(s).serialize()
	return s == t