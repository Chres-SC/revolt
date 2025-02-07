# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *librevolt_cli*         | RPC client functionality used by *revolt-cli* executable |
| *librevolt_common*      | Home for common functionality shared by different executables and libraries. Similar to *librevolt_util*, but higher-level (see [Dependencies](#dependencies)). |
| *librevolt_consensus*   | Stable, backwards-compatible consensus functionality used by *librevolt_node* and *librevolt_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *librevoltconsensus*    | Shared library build of static *librevolt_consensus* library |
| *librevolt_kernel*      | Consensus engine and support library used for validation by *librevolt_node* and also exposed as a [shared library](../shared-libraries.md). |
| *librevoltqt*           | GUI functionality used by *revolt-qt* and *revolt-gui* executables |
| *librevolt_ipc*         | IPC functionality used by *revolt-node*, *revolt-wallet*, *revolt-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *librevolt_node*        | P2P and RPC server functionality used by *revoltd* and *revolt-qt* executables. |
| *librevolt_util*        | Home for common functionality shared by different executables and libraries. Similar to *librevolt_common*, but lower-level (see [Dependencies](#dependencies)). |
| *librevolt_wallet*      | Wallet functionality used by *revoltd* and *revolt-wallet* executables. |
| *librevolt_wallet_tool* | Lower-level wallet functionality used by *revolt-wallet* executable. |
| *librevolt_zmq*         | [ZeroMQ](../zmq.md) functionality used by *revoltd* and *revolt-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *librevolt_consensus* and *librevolt_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`librevolt_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *librevolt_node* code lives in `src/node/` in the `node::` namespace
  - *librevolt_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *librevolt_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *librevolt_util* code lives in `src/util/` in the `util::` namespace
  - *librevolt_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "linear" }}}%%

graph TD;

revolt-cli[revolt-cli]-->librevolt_cli;

revoltd[revoltd]-->librevolt_node;
revoltd[revoltd]-->librevolt_wallet;

revolt-qt[revolt-qt]-->librevolt_node;
revolt-qt[revolt-qt]-->librevoltqt;
revolt-qt[revolt-qt]-->librevolt_wallet;

revolt-wallet[revolt-wallet]-->librevolt_wallet;
revolt-wallet[revolt-wallet]-->librevolt_wallet_tool;

librevolt_cli-->librevolt_common;
librevolt_cli-->librevolt_util;

librevolt_common-->librevolt_util;
librevolt_common-->librevolt_consensus;

librevolt_kernel-->librevolt_consensus;
librevolt_kernel-->librevolt_util;

librevolt_node-->librevolt_common;
librevolt_node-->librevolt_consensus;
librevolt_node-->librevolt_kernel;
librevolt_node-->librevolt_util;

librevoltqt-->librevolt_common;
librevoltqt-->librevolt_util;

librevolt_wallet-->librevolt_common;
librevolt_wallet-->librevolt_util;

librevolt_wallet_tool-->librevolt_util;
librevolt_wallet_tool-->librevolt_wallet;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class revolt-qt,revoltd,revolt-cli,revolt-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *librevolt_wallet* and *librevolt_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code still is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *librevolt_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *librevolt_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *librevolt_common* should serve a similar function as *librevolt_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *librevolt_util* and *librevolt_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for revolt-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *librevolt_kernel* is not supposed to depend on *librevolt_common*, only *librevolt_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *librevolt_kernel* should only depend on *librevolt_util* and *librevolt_consensus*.

- The only thing that should depend on *librevolt_kernel* internally should be *librevolt_node*. GUI and wallet libraries *librevoltqt* and *librevolt_wallet* in particular should not depend on *librevolt_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *librevolt_consensus*, *librevolt_common*, and *librevolt_util*, instead of *librevolt_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *librevoltqt*, *librevolt_node*, *librevolt_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *librevolt_node* to *librevolt_kernel* as part of [The librevoltkernel Project #24303](https://github.com/bitcoin/bitcoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/bitcoin/bitcoin/issues/15732)
