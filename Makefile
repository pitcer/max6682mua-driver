renode_bins:
	$(MAKE) -C renode/bins/ all

run: renode_bins
	cd renode; \
	renode --console --execute "include @scripts/litex.resc; start"; \
	reset
