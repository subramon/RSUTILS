return {
  {
    "neovim/nvim-lspconfig",
    dependencies = {
      -- Automatically install LSPs to stdpath for Neovim
      { "williamboman/mason.nvim", opts = {} },
      "williamboman/mason-lspconfig.nvim",
    },
    config = function()
      -- Ensure clangd and lua_ls are installed automatically
      require("mason-lspconfig").setup({
        ensure_installed = { "clangd", "lua_ls" },
      })

      -- 1. Configure Lua LSP (lua_ls) using Neovim's native API
      vim.lsp.config("lua_ls", {
        settings = {
          Lua = {
            runtime = { version = "LuaJIT" },
            diagnostics = {
              globals = { "vim" }, -- Fixes "Undefined global `vim`" warning
            },
            workspace = {
              checkThirdParty = false,
              library = vim.api.nvim_get_runtime_file("", true),
            },
          },
        },
      })

      -- 2. Configure C/C++ LSP (clangd)
      -- nvim-lspconfig already provides great defaults, so you only need
      -- to call vim.lsp.config if you want to pass custom flags:
      vim.lsp.config("clangd", {
        cmd = {
          "clangd",
          "--background-index",
          "--clang-tidy",
          "--header-insertion=iwyu",
        },
      })

      -- 3. Enable both servers
      vim.lsp.enable({ "lua_ls", "clangd" })
    end,
  },
}

