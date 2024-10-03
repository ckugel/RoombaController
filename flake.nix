{
  description = "Development environment for Zig 0.13.0 from source with GTK 4";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
    in {
      devShell = pkgs.mkShell {
        buildInputs = [
          pkgs.cmake
          pkgs.llvm_18
          pkgs.git
          pkgs.pkg-config
          pkgs.buildPackages.libcxx
          pkgs.libffi
          pkgs.zlib
          pkgs.python3
          pkgs.python3Packages.virtualenv
          pkgs.binutils
          pkgs.clang
          pkgs.openssl

          # GTK 4 and its dependencies
          pkgs.gtk4
          pkgs.glib
          pkgs.cairo
          pkgs.gobject-introspection
        ];

        # Build Zig from source (Zig 0.13.0 from GitHub)
        shellHook = ''
          if [ ! -d "zig" ]; then
            git clone https://github.com/ziglang/zig.git --branch 0.13.0 --depth 1
            cd zig
            cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local/zig -DZIG_PREFER_CLANG_LLVM=ON
            cmake --build build -j$(nproc)
            echo "Zig 0.13.0 has been built and is ready!"
          else
            echo "Zig 0.13.0 is already cloned and built."
            cd zig
          fi

          # Optionally set up environment for GTK 4
          export GTK_PATH=${pkgs.gtk4}/lib
          echo "GTK 4 environment ready!"
        '';
      };
    });
}

