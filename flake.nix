{
  description = "Development environment for Zig 0.13.0 from source with GTK 4";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    zig.url = "github:mitchellh/zig-overlay";
    zls.url = "github:zigtools/zls";
    
    flake-compat = {
      url = "github:edolstra/flake-compat";
      flake = false;
    };
};



outputs =
    { self
    , nixpkgs
    , flake-utils
    , ...
    } @ inputs:
    let
      overlays = [
        # Other overlays
        (final: prev: {
          zigpkgs = inputs.zig.packages.${prev.system};
          zls = inputs.zls.packages.${prev.system}.zls;
        })
      ];

      # Our supported systems are the same supported systems as the Zig binaries
      systems = builtins.attrNames inputs.zig.packages;
    in
    flake-utils.lib.eachSystem systems (
      system:
      let
        pkgs = import nixpkgs { inherit overlays system; };
      in
      rec {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [
            pkgs.zigpkgs."0.13.0"
            pkgs.zls
          ];

          buildInputs = [
            # we need a version of bash capable of being interactive
            # as opposed to a bash just used for building this flake
            # in non-interactive mode
            pkgs.bashInteractive
            pkgs.zlib
          ];

          shellHook = ''
            # once we set SHELL to point to the interactive bash, neovim will
            # launch the correct $SHELL in its :terminal
            export SHELL=${pkgs.bashInteractive}/bin/bash
          '';
        };

        # For compatibility with older versions of the `nix` binary
        devShell = devShells.${system}.default;
  }
  );
}
