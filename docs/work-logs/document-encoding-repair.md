# 정리 문서 인코딩 복구 작업 기록

## 설명

사용자는 일부 정리 문서의 인코딩이 깨져 있으므로 수정해달라고 요청했다.

이번 작업에서는 깨진 한글 문서를 UTF-8 기준의 정상 한글 Markdown 문서로 다시 저장했다.

## 핵심 정리

복구한 주요 문서는 다음과 같다.

```text
AGENTS.md
README.md
PROMPT_REVIEW.md
docs/project-conventions.md
docs/commit-conventions.md
docs/assistant-workflow.md
docs/assistant-response-notes.md
docs/work-logs/cpp-local-ai-chat-ollama.md
docs/work-logs/documentation-folder-separation.md
docs/work-logs/git-commit-log.md
docs/work-logs/prompt-review-workflow.md
docs/work-logs/visual-studio-project-setup.md
```

추가로 `docs/work-logs/`에 잘못 들어가 있던 중복 운영 문서 파일을 제거하고, 루트의 운영 문서를 정상 위치에 복원했다.

검증은 깨진 문자 패턴을 `rg`로 검색하는 방식으로 진행했으며, 복구 대상 문서에서는 더 이상 깨진 문자열 패턴이 검색되지 않았다.

다음에는 문서 인코딩이 다시 깨지지 않도록 `.editorconfig` 또는 문서 저장 규칙을 추가할 수 있다.
