
-- ============================================================================
-- Neovim init.lua - Optimized Systems Development Configuration
-- ============================================================================

require("config.lazy")

-- Set leader keys early
vim.g.mapleader = " "
vim.g.maplocalleader = " "

-- ============================================================================
-- Options
-- ============================================================================
local opt = vim.opt

opt.autowrite = true              -- Enable auto write
opt.clipboard = "unnamedplus"     -- Sync with system clipboard
opt.completeopt = "menu,menuone,noselect"
opt.conceallevel = 2              -- Hide markup for certain filetypes
opt.confirm = true                -- Confirm to save changes before exiting modified buffer
opt.cursorline = true             -- Enable highlighting of the current line
opt.expandtab = true              -- Use spaces instead of tabs
opt.shiftwidth = 4                -- Number of spaces to use for each step of (auto)indent
opt.tabstop = 4                   -- Number of spaces that a <Tab> in the file counts for
opt.softtabstop = 4
opt.smartindent = true            -- Insert indents automatically
opt.formatoptions = "jcroqlnt"    -- Tcqj formatting options
opt.grepformat = "%f:%l:%c:%m"
opt.grepprg = "rg --vimgrep"
opt.hlsearch = true               -- Highlight matched searches
opt.ignorecase = true             -- Ignore case
opt.smartcase = true              -- Don't ignore case with capitals
opt.mouse = "a"                   -- Enable mouse support
opt.number = true                 -- Print line number
opt.relativenumber = true         -- Relative line numbers
opt.pumblend = 10                 -- Popup blend
opt.pumheight = 10                -- Maximum number of items in a popup menu
opt.scrolloff = 8                 -- Lines of context
opt.sidescrolloff = 8             -- Columns of context
opt.sessionoptions = { "buffers", "curdir", "folds", "help", "tabpages", "winsize", "terminal", "localoptions" }
opt.shiftround = true             -- Round indent
opt.signcolumn = "yes"            -- Always show signcolumn
opt.splitbelow = true             -- Put new windows below current
opt.splitright = true             -- Put new windows right of current
opt.termguicolors = true          -- True color support
opt.timeoutlen = 300              -- Lower timeoutlen for faster keymap mapped sequences
opt.undofile = true               -- Enable persistent undo
opt.undolevels = 10000
opt.updatetime = 200              -- Save trigger and cursor hold
opt.wildmode = "longest:full,full" -- Command-line completion mode
opt.wrap = false                  -- Disable line wrap by default

-- ============================================================================
-- Autocommands
-- ============================================================================
local function augroup(name)
  return vim.api.nvim_create_augroup("lazyvim_" .. name, { clear = true })
end

-- Check if file changed when its focus is regained
vim.api.nvim_create_autocmd({ "FocusGained", "TermClose", "TermLeave" }, {
  group = augroup("checktime"),
  command = "checktime",
})

-- Highlight on yank
vim.api.nvim_create_autocmd("TextYankPost", {
  group = augroup("highlight_yank"),
  callback = function()
    vim.highlight.on_yank()
  end,
})

-- Resize splits if window got resized safely across all tabs
vim.api.nvim_create_autocmd({ "VimResized" }, {
  group = augroup("resize_splits"),
  callback = function()
    for _, tab in ipairs(vim.api.nvim_list_tabpages()) do
      local win = vim.api.nvim_tabpage_get_win(tab)
      vim.api.nvim_win_call(win, function()
        vim.cmd("wincmd =")
      end)
    end
  end,
})

-- ============================================================================
-- Keymaps
-- ============================================================================
local map = vim.keymap.set

-- Better up/down navigation on wrapped lines
map({ "n", "x" }, "j", "v:count == 0 ? 'gj' : 'j'", { expr = true, silent = true })
map({ "n", "x" }, "k", "v:count == 0 ? 'gk' : 'k'", { expr = true, silent = true })

-- Window management
map("n", "<C-h>", "<C-w>h", { desc = "Go to left window" })
map("n", "<C-j>", "<C-w>j", { desc = "Go to lower window" })
map("n", "<C-k>", "<C-w>k", { desc = "Go to upper window" })
map("n", "<C-l>", "<C-w>l", { desc = "Go to right window" })

-- Clear search with <esc>
map({ "i", "n" }, "<esc>", "<cmd>noh<cr><esc>", { desc = "Escape and clear search highlight" })

-- Save file
map({ "i", "x", "n", "s" }, "<C-s>", "<cmd>w<cr><esc>", { desc = "Save file" })

-- Better indenting in visual mode
map("v", "<", "<gv")
map("v", ">", ">gv")

-- Diagnostic navigation
map("n", "[d", vim.diagnostic.goto_prev, { desc = "Previous diagnostic" })
map("n", "]d", vim.diagnostic.goto_next, { desc = "Next diagnostic" })

-- for enabling automatic triggering of auto-complete
vim.api.nvim_create_autocmd("LspAttach", {
  callback = function(args)
    local client = vim.lsp.get_client_by_id(args.data.client_id)
    if client and client.server_capabilities.completionProvider then
      -- Enable native autocompletion trigger on type
      vim.lsp.completion.enable(true, client.id, args.buf, { autotrigger = true })
    end
  end,
})
-- for colors
-- vim.g.molokai_original = 1 -- Optional: use the original dark gray background (#1B1D1E) instead of pure black
vim.cmd.colorscheme("molokai")
-- for optional dictironary for auto-complete
-- Load custom dictionary words
vim.opt.dictionary:append(vim.fn.stdpath("config") .. "/dict/c_macros.txt")
-- Optional: include dictionary words in standard <C-n> / <C-p> keyword completion
vim.opt.complete:append("k")

--[==[
-- START trying to fix auto complete 
-- ============================================================================
-- Custom Macro Completion Setup
-- ============================================================================
local c_macros = {
  { word = "cBYE",                  menu = "[Macro]" },
  { word = "fclose_if_non_null",    menu = "[Macro]" },
  { word = "free_if_non_null",      menu = "[Macro]" },
  { word = "go_BYE",                menu = "[Macro]" },
  { word = "mcr_alloc_null_str",    menu = "[Macro]" },
  { word = "mcr_rs_munmap",         menu = "[Macro]" },
  { word = "return_if_fopen_failed",  menu = "[Macro]" },
  { word = "return_if_malloc_failed", menu = "[Macro]" },
  { word = "return_if_null_str",      menu = "[Macro]" },
  { word = "return_if_I_am_the_boss", menu = "[Macro]" },
}

function _G.MacroComplete(findstart, base)
  if findstart == 1 then
    -- Find the start of the current word under the cursor
    local line = vim.api.nvim_get_current_line()
    local col = vim.api.nvim_win_get_cursor(0)[2]
    local start = col
    while start > 0 and line:sub(start, start):match("[%w_]") do
      start = start - 1
    end
    return start
  else
    -- Match typed prefix against c_macros list
    local matches = {}
    for _, item in ipairs(c_macros) do
      if item.word:find("^" .. vim.pesc(base)) then
        table.insert(matches, item)
      end
    end
    return matches
  end
end

-- Set the completion function to user-defined completion
vim.opt.completefunc = "v:lua.MacroComplete"

-- Bind <C-Space> in Insert mode to instantly trigger your custom macro list
vim.keymap.set("i", "<C-Space>", "<C-x><C-u>", { desc = "Trigger C macro completions" })
-- STOP  trying to fix uato complete 

--]==]
vim.api.nvim_create_autocmd("FileType", {
  pattern = "*",
  callback = function()
    vim.opt_local.textwidth = 80
    vim.opt_local.formatoptions:append("t")
  end,
})

-- Bind Alt+q / Esc+q to wrap paragraph without moving the cursor
vim.keymap.set("n", "<M-q>", "gwap", { desc = "Format paragraph (keep cursor)" })

-- remember to sudo apt install xclip
-- The simplest way to enable system clipboard support in Neovim is to add the clipboard option to your Neovim configuration file and install a system clipboard provider if you are on Linux
vim.opt.clipboard = "unnamedplus"

-- Configure clangd
vim.lsp.config('clangd', {
  cmd = { 'clangd' },
  filetypes = { 'c', 'cpp', 'objc', 'objcpp', 'cuda', 'proto' },
  root_markers = { '.clangd', '.clang-tidy', '.clang-format', 'compile_commands.json', 'compile_flags.txt', 'configure.ac', '.git' },
})

-- Configure lua_ls
vim.lsp.config('lua_ls', {
  cmd = { 'lua-language-server' },
  filetypes = { 'lua' },
  root_markers = { '.luarc.json', '.luarc.jsonc', '.luacheckrc', '.stylua.toml', 'stylua.toml', 'selene.toml', 'selene.yml', '.git' },
  settings = {
    Lua = {
      runtime = { version = 'LuaJIT' },
      workspace = {
        checkThirdParty = false,
        library = { vim.env.VIMRUNTIME },
      },
    },
  },
})

-- Now enable them
vim.lsp.enable('clangd')
vim.lsp.enable('lua_ls')
